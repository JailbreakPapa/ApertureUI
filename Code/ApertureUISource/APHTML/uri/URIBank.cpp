#include <APHTML/URI/URIBank.h>
#include <APHTML/URI/URIManager.h>
#include <APHTML/Interfaces/APCPlatform.h>

aperture::uri::URIBank::URIBank(const char* in_uribankname)
{
  m_uribankname = in_uribankname;
}

void aperture::uri::URIBank::SetURIBankName(const char* in_uribankname)
{
  m_uribankname = in_uribankname;
}

const char* aperture::uri::URIBank::GetURIBankName()
{
  return m_uribankname.GetData();
}

UriUriA* aperture::uri::URIBank::FindRawURI(const nsString& in_uri)
{
  return m_uriBank.Find(in_uri).Value();
}

const char* aperture::uri::URIBank::FindStringURI(UriUriA* in_uri)
{
  return m_uriBank.Find(UriObjToStringView(in_uri)).Key().GetData();
}

void aperture::uri::URIBank::AddURI(const nsString& in_uri, UriUriA* in_rawuri)
{
  m_uriBank.Insert(in_uri, in_rawuri);
}

void aperture::uri::URIBank::AddURI(UriUriA* in_uri, nsUInt8 in_accessType /* 0 = OSDependant, 1 = VFS */)
{
  auto UserPlatform = nsSingletonRegistry::GetSingletonInstance<aperture::core::IAPCPlatform>();
  // Before Adding a Uri (In the case there is not a pair), We need to find the full path of the URI (VFS Or OSPATH)
  switch(in_accessType)
  {
    case 0:
      // OSDependant
      URIManager::VerifyURI(in_uri);
      break;
    case 1:
      // VFS
      break;
    default:
      break;
  }
}

void aperture::uri::URIBank::AddURI(const nsString& in_uri, nsUInt8 in_accessType /* 0 = OSDependant, 1 = VFS */)
{
}

void aperture::uri::URIBank::RemoveURI(const nsString& in_uri)
{
}

const char* aperture::uri::URIBank::FindResolvedPath(const nsString& in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::FindOSPath(const nsString& in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::FindOSPath(UriUriA* in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::GetRequestedFileName(const nsString& in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::GetRequestedFileName(UriUriA* in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::GetRequestedFileExtension(const nsString& in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::GetRequestedFileExtension(UriUriA* in_uri)
{
  return nullptr;
}

bool aperture::uri::URIBank::IsContentURI(const nsString& in_uri)
{
  return false;
}

bool aperture::uri::URIBank::IsContentURI(UriUriA* in_uri)
{
  return false;
}
#include <cstring>
#include <sstream>
#include <string>
#include <uriparser/Uri.h>

const char* UriToCString(const UriUriA* uri)
{
  if (!uri)
  {
    return nullptr;
  }

  // Use a string stream to construct the URI
  std::ostringstream oss;

  // Add scheme
  if (uri->scheme.first && uri->scheme.afterLast)
  {
    oss.write(uri->scheme.first, uri->scheme.afterLast - uri->scheme.first);
    oss << ":";
  }

  // Add authority (host, port, user info)
  if (uri->hostText.first && uri->hostText.afterLast)
  {
    oss << "//";

    // Add user info
    if (uri->userInfo.first && uri->userInfo.afterLast)
    {
      oss.write(uri->userInfo.first, uri->userInfo.afterLast - uri->userInfo.first);
      oss << "@";
    }

    // Add host
    oss.write(uri->hostText.first, uri->hostText.afterLast - uri->hostText.first);

    // Add port
    if (uri->portText.first && uri->portText.afterLast)
    {
      oss << ":";
      oss.write(uri->portText.first, uri->portText.afterLast - uri->portText.first);
    }
  }

  // Add path
  const UriPathSegmentA* pathSegment = uri->pathHead;
  while (pathSegment)
  {
    oss << "/";
    if (pathSegment->text.first && pathSegment->text.afterLast)
    {
      oss.write(pathSegment->text.first, pathSegment->text.afterLast - pathSegment->text.first);
    }
    pathSegment = pathSegment->next;
  }

  // Add query
  if (uri->query.first && uri->query.afterLast)
  {
    oss << "?";
    oss.write(uri->query.first, uri->query.afterLast - uri->query.first);
  }

  // Add fragment
  if (uri->fragment.first && uri->fragment.afterLast)
  {
    oss << "#";
    oss.write(uri->fragment.first, uri->fragment.afterLast - uri->fragment.first);
  }

  // Copy the string to a dynamically allocated buffer
  std::string uriString = oss.str();
  char* result = new char[uriString.size() + 1];
  std::strcpy(result, uriString.c_str());
  return result;
}

nsStringView aperture::uri::URIBank::UriObjToStringView(UriUriA* in_uri)
{
  if (URIManager::VerifyURI(in_uri))
  {
    return nsStringView(UriToCString(in_uri));
  }
}
