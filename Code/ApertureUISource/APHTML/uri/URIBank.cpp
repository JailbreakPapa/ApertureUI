#include <APHTML/URI/URIBank.h>

aperture::uri::URIBank::URIBank(const char* in_uribankname)
{
}

void aperture::uri::URIBank::SetURIBankName(const char* in_uribankname)
{
}

const char* aperture::uri::URIBank::GetURIBankName()
{
  return nullptr;
}

UriUriA* aperture::uri::URIBank::FindRawURI(const nsString& in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::FindStringURI(const UriUriA& in_uri)
{
  return nullptr;
}

void aperture::uri::URIBank::AddURI(const nsString& in_uri, UriUriA* in_rawuri)
{
}

void aperture::uri::URIBank::AddURI(const UriUriA& in_uri)
{
}

void aperture::uri::URIBank::AddURI(const nsString& in_uri)
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

const char* aperture::uri::URIBank::FindOSPath(const UriUriA& in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::GetRequestedFileName(const nsString& in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::GetRequestedFileName(const UriUriA& in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::GetRequestedFileExtension(const nsString& in_uri)
{
  return nullptr;
}

const char* aperture::uri::URIBank::GetRequestedFileExtension(const UriUriA& in_uri)
{
  return nullptr;
}

bool aperture::uri::URIBank::IsContentURI(const nsString& in_uri)
{
  return false;
}

bool aperture::uri::URIBank::IsContentURI(const UriUriA& in_uri)
{
  return false;
}
