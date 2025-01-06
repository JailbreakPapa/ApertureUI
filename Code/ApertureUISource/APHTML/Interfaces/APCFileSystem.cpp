#include "APCFileSystem.h"
#include <APHTML/Interfaces/APCFileSystem.h>
#include <APHTML/Interfaces/APCPlatform.h>
#include <APHTML/Localization/ICUCharset.h>
#include <Foundation/IO/OSFile.h>

const char* aperture::core::IAPCFileSystem::GetFullFilePath(const char* urisubpath)
{
  nsStringBuilder temp(m_uiresources);
  nsString urisubpathview;
  temp.Append(urisubpath);
  urisubpathview = temp;
  return urisubpathview;
}

nsOSFileData aperture::core::IAPCFileSystem::GetRawFileHandle(const char* in_filepath, const char* mode)
{
  nsOSFile apcfile;
  nsStringBuilder filep(m_uiresources);
  filep.Append(in_filepath);
  if (apcfile.Open(filep, nsFileOpenMode::Read) == NS_SUCCESS)
  {
    return apcfile.GetFileData();
  }
  return nsOSFileData();
}

const char* aperture::core::IAPCFileSystem::FileCharset(const nsString& in_filepath)
{
  return localization::ICUCharset::GetCharset(GetFileData(in_filepath));
}

aperture::core::CoreBuffer<nsUInt8> aperture::core::IAPCFileSystem::GetFileData(const char* in_filepath, EFileType type)
{
  nsOSFile apcfile;
  nsStringBuilder filep(m_uiresources);
  filep.Append(in_filepath);

  if (apcfile.Open(filep, nsFileOpenMode::Read) != NS_SUCCESS)
  {
    nsLog::SeriousWarning("Failed to read file data!");
    return CoreBuffer<nsUInt8>();
  }

  nsDynamicArray<nsUInt8> filedata;
  apcfile.ReadAll(filedata);

  return aperture::core::CoreBuffer<nsUInt8>(filedata.GetData(), filedata.GetCount());
}
bool aperture::core::IAPCFileSystem::RequestCreateFile(const char* in_filepath, core::CoreBuffer<nsUInt8> out_filedata, EFileType type)
{
  nsOSFile apcfile;
  nsStringBuilder filep(m_uiresources);
  filep.Append(in_filepath);
  if (apcfile.Open(filep, nsFileOpenMode::Write) != NS_SUCCESS)
  {
    nsLog::SeriousWarning("Failed to create file!");
    return false;
  }
  apcfile.Write(out_filedata.get(), out_filedata.size());
  return true;
}
const char* aperture::core::IAPCFileSystem::GetFileMimeType(const char* in_filepath)
{
  nsOSFile apcfile;
  if (apcfile.Open(in_filepath, nsFileOpenMode::Read) == NS_SUCCESS)
  {
    nsLog::SeriousWarning("Failed to read Mime-type!");
    return "BAD-MIME";
  }
  std::string filename = apcfile.GetOpenFileName().GetStartPointer();
  size_t pos = filename.find_last_of('.');
  if (pos == std::string::npos)
  {
    return "BAD-MIME";
  }
  std::string ext = filename.substr(pos);
  if (ext == ".html")
  {
    return "text/html";
  }
  else if (ext == ".xml")
  {
    return "application/xml";
  }
  else if (ext == ".css")
  {
    return "text/css";
  }
  else if (ext == ".js")
  {
    return "text/javascript";
  }
  else if (ext == ".png")
  {
    return "image/png";
  }
  else if (ext == ".jpg" || ext == ".jpeg")
  {
    return "image/jpeg";
  }
  else if (ext == ".gif")
  {
    return "image/gif";
  }
  else if (ext == ".svg")
  {
    return "image/svg+xml";
  }
  else if (ext == ".ico")
  {
    return "image/x-icon";
  }
  else if (ext == ".json")
  {
    return "application/json";
  }
  else if (ext == ".pdf")
  {
    return "application/pdf";
  }
  else if (ext == ".zip")
  {
    return "application/zip";
  }
  else if (ext == ".tar")
  {
    return "application/x-tar";
  }
  else if (ext == ".gz")
  {
    return "application/gzip";
  }
  else if (ext == ".rar")
  {
    return "application/x-rar-compressed";
  }
  else if (ext == ".7z")
  {
    return "application/x-7z-compressed";
  }
  return "BAD-MIME";
}
