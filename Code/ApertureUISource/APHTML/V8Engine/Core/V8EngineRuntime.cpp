#include "V8EngineRuntime.h"
#include <APHTML/APEngine.h>
#include <APHTML/V8Engine/Core/V8EngineRuntime.h>
#include <APHTML/V8Engine/System/Internal/V8Helpers.h>
#include <Foundation/Utilities/Compression.h>

NS_IMPLEMENT_SINGLETON(aperture::v8::V8EEngineRuntime);

nsResult aperture::v8::V8EEngineRuntime::RequestScriptToBeCompiledAndRan(const aperture::core::CoreBuffer<nsUInt8>& in_Script, bool in_bRunImmediately = false, const char* in_pScriptName = "default_apui_js_script")
{
  auto UserPlatform = nsSingletonRegistry::GetSingletonInstance<aperture::core::IAPCPlatform>();
  auto UserRuntime = nsSingletonRegistry::GetSingletonInstance<aperture::v8::V8EEngineRuntime>();
  if (in_Script.size() == 0)
  {
    UserPlatform->GetLoggingSystem()->LogWarning(
      "V8EEngineRuntime::RequestScriptToBeCompiledAndRan: Script is empty, cannot compile and run!");
    return NS_FAILURE;
  }
  if (in_bRunImmediately)
  {
    nsSharedPtr<V8QuickScriptTask> task = nsMakeShared<V8QuickScriptTask>(in_Script, in_pScriptName, UserRuntime);
    UserRuntime->m_TaskGroups.PushBack(nsTaskSystem::StartSingleTask(task, nsTaskPriority::EarlyThisFrame));
  }
  UserRuntime->m_ScriptsToCompileAndRun.PushBack(in_Script);
  return NS_SUCCESS;
}

nsHybridArray<std::pair<::v8::SnapshotCreator*, ::v8::StartupData>, 1>
aperture::v8::V8EEngineRuntime::GetSnapshots() const
{
  return m_Snapshots;
}

aperture::v8::V8EEngineRuntime::~V8EEngineRuntime()
{
  Cleanup_TaskGroups();
  Cleanup_Isolates();
  Cleanup_Snapshots();
  Cleanup_Scripts();
  m_Isolates.Clear();
  m_IsolatesToCache.Clear();
  m_IsolatesToDestroy.Clear();
  m_Snapshots.Clear();
  m_SnapshotData.Clear();
  m_ScriptsToCompileAndRun.Clear();
  m_TaskGroups.Clear();
  NS_DEFAULT_DELETE(m_pV8EngineMain);
}

nsResult aperture::v8::V8EEngineRuntime::CacheBoundObjects(::v8::Isolate* in_isolate, ::v8::Local<::v8::FunctionTemplate> in_FuncTemplate, const char* in_pCacheName, bool in_bWriteImmediately)
{
  auto UserPlatform = nsSingletonRegistry::GetSingletonInstance<aperture::core::IAPCPlatform>();
  auto UserRuntime = nsSingletonRegistry::GetSingletonInstance<aperture::v8::V8EEngineRuntime>();
  nsUniquePtr<::v8::SnapshotCreator> snapshot_ = nsMakeUnique<::v8::SnapshotCreator>(in_isolate);
  auto startupData = std::make_unique<::v8::StartupData>();
  if (ApertureSDK::IsCachingAllowed() == false)
  {
    UserPlatform->GetLoggingSystem()->LogWarning(
      "V8EEngineRuntime::CacheBoundObjects: Caching is not allowed, cannot cache bound objects! (To Change this, set the value in ApertureSDK::SetIsCachingAllowed, and Restart your IAPCPlatform.)");
    return NS_FAILURE;
  }


  if (!in_isolate)
  {
    // Attempt to acquire a random isolate from the available cache.
    UserPlatform->GetLoggingSystem()->LogWarning(
      "V8EEngineRuntime::CacheBoundObjects: Isolate is nullptr. Attempting to grab one for binding...");

    if (UserRuntime->m_IsolatesToCache.GetCount() > 0)
    {
      in_isolate = UserRuntime->m_IsolatesToCache[0];
    }
    else
    {
      UserPlatform->GetLoggingSystem()->LogWarning(
        "V8EEngineRuntime::CacheBoundObjects: No isolates to bind to!");
      return NS_FAILURE;
    }
  }

  if (in_isolate->IsDead() || in_FuncTemplate.IsEmpty())
  {
    UserPlatform->GetLoggingSystem()->LogWarning(
      "V8EEngineRuntime::CacheBoundObjects: Isolate is dead or FunctionTemplate is empty, cannot cache bound objects!");
    return NS_FAILURE;
  }

  snapshot_->SetDefaultContext(in_isolate->GetCurrentContext());
  snapshot_->AddData(in_FuncTemplate);

  if (in_bWriteImmediately)
  {
    *startupData = snapshot_->CreateBlob(::v8::SnapshotCreator::FunctionCodeHandling::kKeep);
    if (!startupData->data)
    {
      UserPlatform->GetLoggingSystem()->LogWarning(
        "V8EEngineRuntime::CacheBoundObjects: Failed to create snapshot blob!");
      return NS_FAILURE;
    }

    aperture::core::CoreBuffer<nsUInt8> buffer(startupData->data, startupData->raw_size);
    if (UserPlatform->GetFileSystem()->RequestCreateFile(in_pCacheName, buffer))
    {
      UserPlatform->GetLoggingSystem()->LogInfo(
        "V8EEngineRuntime::CacheBoundObjects: Successfully cached bound objects!");
      return NS_SUCCESS;
    }
  }

  auto pair = std::make_pair(snapshot_.Borrow(), *startupData);
  UserRuntime->m_Snapshots.PushBack(pair);
  return NS_SUCCESS;
}

void aperture::v8::V8EEngineRuntime::PrepareRuntime()
{
}
