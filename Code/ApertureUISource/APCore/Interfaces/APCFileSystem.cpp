#include "APCFileSystem.h"
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

nsString aperture::core::IAPCFileSystem::FileCharset(const nsString& in_filepath)
{
  // TODO: I will implement this later.
  return nsString();
}

const char* aperture::core::IAPCFileSystem::GetFileMimeType(const char* in_filepath)
{
  nsOSFile apcfile;
  if (apcfile.Open(in_filepath, nsFileOpenMode::Read) == NS_SUCCESS)
  {
    nsLog::SeriousWarning("Failed to read Mime-type!");
    return "BAD-MIME";
  }
  if (apcfile.GetOpenFileName().FindSubString(".html"))
  {
    return "text/html";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".css"))
  {
    return "text/css";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".js"))
  {
    return "text/javascript";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".png"))
  {
    return "image/png";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".jpg"))
  {
    return "image/jpeg";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".jpeg"))
  {
    return "image/jpeg";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".gif"))
  {
    return "image/gif";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".svg"))
  {
    return "image/svg+xml";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".ico"))
  {
    return "image/x-icon";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".json"))
  {
    return "application/json";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".xml"))
  {
    return "application/xml";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".pdf"))
  {
    return "application/pdf";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".zip"))
  {
    return "application/zip";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".tar"))
  {
    return "application/x-tar";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".gz"))
  {
    return "application/gzip";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".rar"))
  {
    return "application/x-rar-compressed";
  }
  else if (apcfile.GetOpenFileName().FindSubString(".7z"))
  {
    return "application/x-7z-compressed";
  }
  return "BAD-MIME";
}
