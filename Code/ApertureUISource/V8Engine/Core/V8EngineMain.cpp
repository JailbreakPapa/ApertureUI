#include <V8Engine/Core/V8EngineMain.h>
#include <APCore/APEngine.h>

bool aperture::v8::jobsystem::V8EEngineMain::InitializeV8Engine(const char* p_ccResources)
{
  if (p_ccResources != nullptr)
  {
    nsStringBuilder* finalResourceDir = nullptr;
    std::string s = nsOSFile::GetApplicationDirectory().GetStartPointer();
    nsStringBuilder finalicuDatafile = s.c_str();
    finalicuDatafile.AppendPath("/resources", "/icudtl.dat");
    if (::v8::V8::InitializeICUDefaultLocation(s.c_str(), finalicuDatafile.GetData()))
    {
      ::v8::V8::InitializeExternalStartupData(p_ccResources);
      m_pV8EPlatform = std::make_unique<V8EPlatform>(ApertureSDK::GetScriptThreadCount(), ::v8::platform::IdleTaskSupport::kEnabled, ::v8::platform::InProcessStackDumping::kEnabled, nullptr, ::v8::platform::PriorityMode::kApply);

      ::v8::V8::InitializePlatform(GetV8EEnginePlatform());
      if (::v8::V8::Initialize())
      {
        nsLog::Success("V8Engine: Successfully Initialized V8.");
        return true;
      }
    }
    else
    {
      nsLog::Error("V8Engine: Failed to initialize ICU Data file.");
      return false;
    }
  }
  else
  {
    nsLog::Error("V8Engine: Resource path provided (p_ccResources) is nullptr.");
    return false;
  }
}
aperture::v8::jobsystem::V8EJobManager* aperture::v8::jobsystem::V8EEngineMain::GetV8EJobManager()
{
  return m_pV8EJobManager.get();
}

aperture::v8::jobsystem::V8EPlatform* aperture::v8::jobsystem::V8EEngineMain::GetV8EEnginePlatform()
{
  return m_pV8EPlatform.get();
}
