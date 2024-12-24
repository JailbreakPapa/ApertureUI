#include <APCore/APEngine.h>
#include <V8Engine/Core/V8EngineMain.h>
#include "V8EngineMain.h"

bool aperture::v8::V8EEngineMain::InitializeV8Engine(const char* p_ccResources)
{
  nsStringBuilder* finalResourceDir = nullptr;
  const char* s = nsOSFile::GetApplicationDirectory().GetStartPointer();
  nsStringBuilder finalicuDatafile = s;

  if (p_ccResources == nullptr)
  {
    nsLog::SeriousWarning("V8Engine: Resource path provided (p_ccResources) is nullptr. Attempting to find resources in the current directory.");
    if (nsOSFile::ExistsDirectory(finalicuDatafile.GetData()))
    {
      finalicuDatafile.AppendPath("/resources", "/icudtl.dat");
      if (nsOSFile::ExistsFile(finalicuDatafile.GetData()))
      {
        nsLog::Success("V8Engine: Successfully found resources in the current directory.");
        p_ccResources = finalicuDatafile.GetData();
      }
      nsLog::Error("V8Engine: Failed to find resources in the current directory.");
      return false;
    }
  }

  if (::v8::V8::InitializeICUDefaultLocation(s, p_ccResources))
  {
    ::v8::V8::InitializeExternalStartupData(p_ccResources);
    m_pV8EPlatform = std::make_unique<jobsystem::V8EPlatform>(ApertureSDK::GetScriptThreadCount(), ::v8::platform::IdleTaskSupport::kEnabled, ::v8::platform::InProcessStackDumping::kEnabled, nullptr, ::v8::platform::PriorityMode::kApply);

    ::v8::V8::InitializePlatform(GetV8EEnginePlatform());
    if (::v8::V8::Initialize())
    {
      m_pV8EJobManager = std::make_unique<jobsystem::V8EJobManager>();
      nsLog::Success("V8Engine: Successfully Initialized V8.");
      return true;
    }
  }
  nsLog::Error("V8Engine: Failed to initialize ICU Data file.");
  return false;
}
void aperture::v8::V8EEngineMain::ShutdownV8Engine()
{
  ::v8::V8::Dispose();
  ::v8::V8::DisposePlatform();
  m_pV8EJobManager->Shutdown();
  m_pV8EJobManager.reset();
  m_pV8EPlatform.reset();
  nsLog::Success("V8Engine: Successfully Shutdown V8.");
}
aperture::v8::jobsystem::V8EJobManager* aperture::v8::V8EEngineMain::GetV8EJobManager()
{
  return m_pV8EJobManager.get();
}

aperture::v8::jobsystem::V8EPlatform* aperture::v8::V8EEngineMain::GetV8EEnginePlatform()
{
  return m_pV8EPlatform.get();
}
