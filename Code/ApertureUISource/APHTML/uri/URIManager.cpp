#include <APHTML/URI/URIManager.h>

const char* aperture::uri::URIManager::ConvertOSPathToURI(const char* in_uri)
{
  auto UserPlatform = nsSingletonRegistry::GetSingletonInstance<aperture::core::IAPCPlatform>();
  const int bytesNeeded = 8 + 3 * strlen(in_uri) + 1;
  char* absUri = static_cast<char*>(UserPlatform->GetAllocatorSystem()->Alloc(bytesNeeded * sizeof(char), 0));
  if (uriWindowsFilenameToUriStringA(in_uri, absUri) != URI_SUCCESS)
  {
    /* Failure */
    UserPlatform->GetAllocatorSystem()->Free(absUri);
    UserPlatform->GetLoggingSystem()->LogError("URIManager::ConvertOSPathToURI: Failed to convert OS Path to URI!");
  }
  return absUri;
}

bool aperture::uri::URIManager::VerifyURI(const char* in_uri)
{
  auto UserPlatform = nsSingletonRegistry::GetSingletonInstance<aperture::core::IAPCPlatform>();
  UriUriA uri;
  const char* errorpos;
  if (uriParseSingleUriA(&uri, in_uri, &errorpos) != URI_SUCCESS)
  {
    UserPlatform->GetLoggingSystem()->LogError("URIManager::VerifyURI: Uri Is Invalid. Error Pos: {%s}", errorpos);
    return false;
  }
  uriFreeUriMembersA(&uri);
  return true;
}

const char* aperture::uri::URIManager::GetScheme(const UriUriA& in_uri)
{
  return in_uri.scheme.first;
}

const char* aperture::uri::URIManager::GetHost(const UriUriA& in_uri)
{
  return in_uri.hostText.first;
}

const char* aperture::uri::URIManager::GetPath(const UriUriA& in_uri)
{
  return in_uri.pathHead->text.first;
}

const char* aperture::uri::URIManager::GetQuery(const UriUriA& in_uri)
{
  return in_uri.query.first;
}

const char* aperture::uri::URIManager::GetFragment(const UriUriA& in_uri)
{
  return in_uri.fragment.first;
}

const char* aperture::uri::URIManager::GetPort(const UriUriA& in_uri)
{
  return in_uri.portText.first;
}
