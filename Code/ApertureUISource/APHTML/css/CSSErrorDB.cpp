#include <APHTML/Interfaces/APCPlatform.h>
#include <APHTML/css/CSSErrorDB.h>

NS_IMPLEMENT_SINGLETON(aperture::css::CSSErrorDatabase);


void aperture::css::CSSErrorDatabase::ClearErrors()
{
  m_errorData.Clear();
}

void aperture::css::CSSErrorDatabase::PrintErrors()
{
  for (auto& error : m_errorData)
  {
    nsLog::Error("CSS Error: %s\nFile: %s\nLine: %d\nColumn: %d", error.m_errorMessage, error.m_fileData.m_filePath, error.m_lineNumber, error.m_columnNumber);
  }
}
