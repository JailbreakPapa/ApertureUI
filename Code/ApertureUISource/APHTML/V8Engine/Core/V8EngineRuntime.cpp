#include <APHTML/APEngine.h>
#include <APHTML/V8Engine/Core/V8EngineMain.h>
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

nsResult aperture::v8::V8EEngineRuntime::ProcessAndTickEngine()
{
  auto UserPlatform = nsSingletonRegistry::GetSingletonInstance<aperture::core::IAPCPlatform>();

  if (m_ScriptsToCompileAndRun.GetCount() > 0)
  {
    CompileAndRunScripts();
  }

  return NS_SUCCESS;
}

aperture::v8::V8EEngineRuntime::~V8EEngineRuntime()
{
  ShutdownRuntime();
}

void aperture::v8::V8EEngineRuntime::InitializeRuntime(V8EEngineMain* pEngineMain)
{
  auto UserPlatform = nsSingletonRegistry::GetSingletonInstance<aperture::core::IAPCPlatform>();
  if (!pEngineMain)
  {
    UserPlatform->GetLoggingSystem()->LogError(
      "V8EEngineRuntime::InitializeRuntime: V8EEngineMain is nullptr, cannot initialize runtime!");
    return;
  }
  m_pV8EngineMain = pEngineMain;
  im_RuntimeStatus = NS_SUCCESS;
}

aperture::v8::V8EEngineMain* aperture::v8::V8EEngineRuntime::GetV8EngineMain()
{
  return m_pV8EngineMain;
}

void aperture::v8::V8EEngineRuntime::ShutdownRuntime()
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

nsResult aperture::v8::V8EEngineRuntime::GetRuntimeStatus() const
{
  return im_RuntimeStatus;
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
  Cleanup_Isolates();
  Cleanup_Snapshots();
  Cleanup_Scripts();
}

nsResult aperture::v8::V8EEngineRuntime::CompileAndRunScripts()
{
  auto UserPlatform = nsSingletonRegistry::GetSingletonInstance<aperture::core::IAPCPlatform>();
  std::vector<core::CoreBuffer<nsUInt8>> filedatafiles;
  if (m_ScriptsToCompileAndRun.GetCount() == 0)
  {
    UserPlatform->GetLoggingSystem()->LogInfo(
      "V8EEngineRuntime::CompileAndRunScripts: No scripts to compile and run! Attempting to Request from the User...");
    if (!UserPlatform->GetFileSystem()->RequestURIResolve("apui://script/aperturejs_sdk.js", "", filedatafiles, aperture::core::IAPCFileSystem::EFileType::OSDependant, aperture::core::IAPCFileSystem::ERequestType::Script_JS, aperture::core::IAPCFileSystem::ERequestAmmount::GlobAll))
    {
      UserPlatform->GetLoggingSystem()->LogError(
        "V8EEngineRuntime::CompileAndRunScripts: Failed to request script(s) from the user! (Is Your Overriden RequestURIResolve function handling the UIResources path?)");
      return NS_FAILURE;
    }
    for (auto& filedata : filedatafiles)
    {
      if(filedata.size() == 0)
      {
        UserPlatform->GetLoggingSystem()->LogError(
          "V8EEngineRuntime::CompileAndRunScripts: Script is empty, cannot compile and run!");
        return NS_FAILURE;
      }
      m_ScriptsToCompileAndRun.PushBack(filedata);
    }
  }

  for (auto& script : m_ScriptsToCompileAndRun)
  {
    nsSharedPtr<V8QuickScriptTask> task = nsMakeShared<V8QuickScriptTask>(script, "apui_managed_script", this);
    m_TaskGroups.PushBack(nsTaskSystem::StartSingleTask(task, nsTaskPriority::EarlyThisFrame));
  }
  return NS_SUCCESS;
}

void aperture::v8::V8EEngineRuntime::Cleanup_TaskGroups()
{
}

void aperture::v8::V8EEngineRuntime::Cleanup_Isolates()
{
}

void aperture::v8::V8EEngineRuntime::Cleanup_Snapshots()
{
}

void aperture::v8::V8EEngineRuntime::Cleanup_Scripts()
{
}

void aperture::v8::V8EEngineRuntime::V8QuickScriptTask::Execute()
{
  // Execute the script.
  // (Mikael A.): Attempt to get the current isolate. Due to the multithreaded nature of the Task System, the Thread we are on may not have an isolate, so we will create one ourself if needed.
  ::v8::Isolate* isolate = ::v8::Isolate::GetCurrent();
  if (!isolate || isolate->IsDead())
  {
    ::v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = ::v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    if (m_pRuntime->GetSnapshots().GetCount() > 0)
    {
      for (auto& snapshot : m_pRuntime->GetSnapshots())
      {
        if (snapshot.second.IsValid())
        {
          create_params.snapshot_blob = &snapshot.second;
        }
      }
    }
    isolate = ::v8::Isolate::New(create_params);
    auto script = helpers::compile(isolate->GetCurrentContext(), helpers::to_string(isolate, (const char*)m_Script.get()), m_ScriptName);
    if (!script.ToLocalChecked()->Run(isolate->GetCurrentContext()).ToLocalChecked().IsEmpty())
    {
      nsLog::Success("V8QuickScriptTask::Execute: Successfully ran script: {0}", m_ScriptName);
    }
    nsLog::Error("V8QuickScriptTask::Execute: Failed to run script: {0}", m_ScriptName);
  }
}

aperture::v8::V8EEngineRuntime::V8QuickScriptTask::~V8QuickScriptTask()
{
  delete[] m_Script.get();
  delete[] m_ScriptName;
}

nsTaskGroupID aperture::v8::V8EEngineRuntime::V8QuickScriptTask::GetTaskGroupID() const
{
  return m_TaskGroupID;
}
