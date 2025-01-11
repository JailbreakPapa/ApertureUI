#include <APHTML/APEngine.h>
#include <APHTML/V8Engine/Core/V8EngineMain.h>

bool aperture::v8::V8EEngineMain::InitializeV8Engine(const char* p_ccResources)
{
  const char* s = nsOSFile::GetApplicationPath().GetStartPointer();
  nsStringBuilder finalSnapshot = s;
  nsStringBuilder finalicuDatafile = s;
  if (!p_ccResources)
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
    }
  }

  if (::v8::V8::InitializeICUDefaultLocation(finalicuDatafile, p_ccResources))
  {
    finalSnapshot.AppendPath("/resources", "/snapshot_blob.bin");
    SnapshotFile = finalSnapshot.GetData();
    ::v8::V8::InitializeExternalStartupDataFromFile(finalSnapshot.GetData());
    m_pV8EPlatform = nsMakeUnique<::v8::Platform>(::v8::platform::NewDefaultPlatform());
    ::v8::V8::InitializePlatform(m_pV8EPlatform.Borrow());
    if (::v8::V8::Initialize())
    {
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
  m_pV8EPlatform.Clear();
  nsLog::Success("V8Engine: Successfully Shutdown V8.");
}
