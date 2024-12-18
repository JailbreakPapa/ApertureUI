#include <ApertureHTMLTest/Utils/APCPlatformSetup.h>
#include <Foundation/IO/FileSystem/FileSystem.h>
#include <Foundation/IO/OSFile.h>

aperture::core::APCBuffer APHTMLTEST_APCFileSystem::GetFileData(const char* in_filepath)
{
  nsOSFile apcfile;
  nsStringBuilder filep(this->m_uiresources);
  filep.Append(in_filepath);
  apcfile.Open(filep, nsFileOpenMode::Read);
  if (apcfile.IsOpen())
  {
    nsDataBuffer dataBuffer;
    apcfile.ReadAll(dataBuffer);
    apcfile.Close();
    return dataBuffer;
  }
}

bool APHTMLTEST_APCFileSystem::FileExists(const nsString& in_filepath)
{
  return nsFileSystem::ExistsFile(in_filepath);
}

nsString APHTMLTEST_APCFileSystem::FileCharset(const nsString& in_filepath)
{
  /// Return UTF-8 as default charset for now.
  return "utf-8";
}
