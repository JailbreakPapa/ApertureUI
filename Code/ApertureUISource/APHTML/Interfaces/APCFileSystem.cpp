#include "APCFileSystem.h"
#include <APHTML/Interfaces/APCFileSystem.h>
#include <APHTML/Interfaces/APCPlatform.h>
#include <APHTML/Localization/ICUCharset.h>
#include <Foundation/IO/Archive/ArchiveReader.h>
#include <Foundation/IO/MemoryStream.h>
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


bool aperture::core::IAPCFileSystem::EvaluateVFSInternal(const char* in_uri, const char* in_patharchivepath, std::vector<core::CoreBuffer<nsUInt8>>& out_filedatafiles, EFileType type, ERequestType filetype, ERequestAmmount ammount)
{
  nsUInt32 uiIndex = 0;
  nsArchiveReader reader;
  if (reader.OpenArchive(in_patharchivepath) == NS_FAILURE)
  {
    nsLog::SeriousWarning("Failed to open archive!");
    return false;
  }
  auto toc = reader.GetArchiveTOC();
  if (uiIndex = toc.FindEntry(in_uri) == nsInvalidIndex)
  {

    nsLog::SeriousWarning("Failed to find entry! Retrying with marker(SDK) files.");
    // NOTE(Mikael A.): Attempt to find the entry in the archive with the required supplied files. If we fail to find the entry, we will return false.
    switch(filetype)
    {
      case (ERequestType::Script_JS):
      {
        if (uiIndex = toc.FindEntry("aperturejs_sdk.js") == nsInvalidIndex)
        {
          nsLog::SeriousWarning("Failed to find entry for SDK files!");
          return false;
        }
        break;
      }
      case (ERequestType::CSS):
      {
        if (uiIndex = toc.FindEntry("aperturecss_sdk.css") == nsInvalidIndex)
        {
          nsLog::SeriousWarning("Failed to find entry for SDK files!");
          return false;
        }
        break;
      }
      case (ERequestType::XML):
      {
        if (uiIndex = toc.FindEntry("aperturexml_sdk.xml") == nsInvalidIndex)
        {
          nsLog::SeriousWarning("Failed to find entry for SDK files!");
          return false;
        }
        break;
      }
      case (ERequestType::HTML):
      {
        if (uiIndex = toc.FindEntry("aperturehtml_sdk.html") == nsInvalidIndex)
        {
          nsLog::SeriousWarning("Failed to find entry for SDK files!");
          return false;
        }
        break;
      }
      case (ERequestType::JSON):
      {
        if (uiIndex = toc.FindEntry("aperturejson_sdk.json") == nsInvalidIndex)
        {
          nsLog::SeriousWarning("Failed to find entry for SDK files!");
          return false;
        }
        break;
      }
    }
  }
  auto sreader = reader.CreateEntryReader(uiIndex);
  if (sreader->ReadBytes(out_filedatafiles[0].get(), toc.m_Entries[uiIndex].m_uiUncompressedDataSize) == toc.m_Entries[uiIndex].m_uiUncompressedDataSize)
  {
    return true;
  }
  return false;
}
const char* aperture::core::IAPCFileSystem::FileCharset(const nsString& in_filepath)
{
  return localization::ICUCharset::GetCharset(GetFileData(in_filepath));
}

bool aperture::core::IAPCFileSystem::RequestURIResolve(const char* in_uri, const char* in_patharchivepath, std::vector<core::CoreBuffer<nsUInt8>>& out_filedatafiles, EFileType type, ERequestType filetype, ERequestAmmount ammount)
{
  switch (type)
  {
    case (EFileType::OSDependant):
    {
      nsOSFile apcfile;
      nsStringBuilder filep(m_uiresources);
      filep.Append(in_uri);
      if (apcfile.Open(filep, nsFileOpenMode::Read) != NS_SUCCESS)
      {
        nsLog::SeriousWarning("Failed to read file data!");
        return false;
      }
      nsDynamicArray<nsUInt8> filedata;
      apcfile.ReadAll(filedata);
      out_filedatafiles.push_back(core::CoreBuffer<nsUInt8>(filedata.GetData(), filedata.GetCount()));
      return true;
    }
    case (EFileType::VFS):
    {
      // TODO(Mikael A.): ArchiveSystem Doesnt Allow Us to Effectively Glob Files. We Need to Implement a Better Way to Handle This.
      return EvaluateVFSInternal(in_uri, in_patharchivepath, out_filedatafiles, type, filetype, ammount);
    }
  }
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
