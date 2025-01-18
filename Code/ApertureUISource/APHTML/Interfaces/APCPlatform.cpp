#include <APHTML/Interfaces/APCPlatform.h>
#include <APHTML/APEngine.h>
#include <string>
#include "APCPlatform.h"

NS_IMPLEMENT_SINGLETON(aperture::core::IAPCPlatform);
NS_ENUMERABLE_CLASS_IMPLEMENTATION(aperture::core::IAPCPlatform);

bool aperture::core::IAPCPlatform::InitializePlatform(const char* licensekey)
{
  // NOTE(Mikael A.): Removing All CryptoLens API calls for FOSS release.
  // TODO: #1: Implement Resources Check(https://github.com/WatchDogStudios/ApertureUI/issues/1).
  nsLog::Info("IAPCPlatform::InitializePlatform: Platform Initialized! Aperture Version: {0}", ApertureSDK::GetSDKVersion());
  m_bEngineStatus = true;
  m_Stopwatch.Resume();
  return true;
}

void aperture::core::IAPCPlatform::SetFileSystem(const IAPCFileSystem& in_filesystem)
{
  NS_LOCK(lockermt);
  m_filesystem = const_cast<IAPCFileSystem*>(&in_filesystem);
}

void aperture::core::IAPCPlatform::SetMemoryAllocator(const IAPCMemoryAllocator& in_memorysystem)
{
  NS_LOCK(lockermt);
  m_memoryallocator = const_cast<IAPCMemoryAllocator*>(&in_memorysystem);
}

void aperture::core::IAPCPlatform::SetLoggingSystem(const IAPCLoggingSystem& in_loggingsystem)
{
  if (&in_loggingsystem)
  {
    NS_LOCK(lockermt);
    m_loggingsystem = const_cast<IAPCLoggingSystem*>(&in_loggingsystem);
  }
}


void aperture::core::IAPCPlatform::KillPlatform()
{
  NS_LOCK(lockermt);
  m_bEngineStatus = false;
  m_Stopwatch.StopAndReset();
  SetFileSystem(nullptr);
  SetMemoryAllocator(nullptr);
  SetLoggingSystem(nullptr);
}

bool aperture::core::IAPCPlatform::IsEngineReady()
{
  return m_bEngineStatus;
}


