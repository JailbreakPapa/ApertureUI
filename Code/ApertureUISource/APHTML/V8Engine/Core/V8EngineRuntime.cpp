#include <APHTML/V8Engine/Core/V8EngineRuntime.h>
#include <Foundation/Utilities/Compression.h>
#include "V8EngineRuntime.h"

nsResult aperture::v8::V8EEngineRuntime::CacheBoundObjects(::v8::Isolate* in_isolate, ::v8::Local<::v8::FunctionTemplate> in_FuncTemplate, const char* in_pCacheName, bool in_bWriteImmediately)
{
  auto UserPlatform = nsSingletonRegistry::GetSingletonInstance<aperture::core::IAPCPlatform>();
  auto UserRuntime = nsSingletonRegistry::GetSingletonInstance<aperture::v8::V8EEngineRuntime>();
  nsUniquePtr<::v8::SnapshotCreator> snapshot_ = nsMakeUnique<::v8::SnapshotCreator>(in_isolate);
  auto startupData = std::make_unique<::v8::StartupData>();

  if (!in_isolate)
  {
    // Attempt to acquire a random isolate from the available cache.
    UserPlatform->GetLoggingSystem()->LogWarning(
      "V8EEngineRuntime::CacheBoundObjects: Isolate is nullptr. Attempting to grab one for binding..."
    );

    if (UserRuntime->m_IsolatesToCache.GetCount() > 0)
    {
      in_isolate = UserRuntime->m_IsolatesToCache[0];
    }
    else
    {
      UserPlatform->GetLoggingSystem()->LogWarning(
        "V8EEngineRuntime::CacheBoundObjects: No isolates to bind to!"
      );
      return NS_FAILURE;
    }
  }

  if (in_isolate->IsDead() || in_FuncTemplate.IsEmpty())
  {
    UserPlatform->GetLoggingSystem()->LogWarning(
      "V8EEngineRuntime::CacheBoundObjects: Isolate is dead or FunctionTemplate is empty, cannot cache bound objects!"
    );
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
        "V8EEngineRuntime::CacheBoundObjects: Failed to create snapshot blob!"
      );
      return NS_FAILURE;
    }

    aperture::core::CoreBuffer<nsUInt8> buffer(startupData->data, startupData->raw_size);
    if (UserPlatform->GetFileSystem()->RequestCreateFile(in_pCacheName, buffer))
    {
      UserPlatform->GetLoggingSystem()->LogInfo(
        "V8EEngineRuntime::CacheBoundObjects: Successfully cached bound objects!"
      );
      return NS_SUCCESS;
    }
  }

  auto pair = std::make_pair(snapshot_.Borrow(), *startupData);
  UserRuntime->m_Snapshots.PushBack(pair);
  return NS_SUCCESS;
}
