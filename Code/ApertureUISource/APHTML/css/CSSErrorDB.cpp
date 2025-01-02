#include <APHTML/Interfaces/APCPlatform.h>
#include <APHTML/css/CSSErrorDB.h>

void aperture::css::CSSErrorDatabase::AddError(CSSErrorType in_errortype, CSSParsedFileData& in_fileData, const char* in_errorMessage, int in_lineNumber, int in_columnNumber)
{
  CSSErrorData errorData;
  errorData.m_fileData = in_fileData;
  errorData.m_errorMessage = in_errorMessage;
  errorData.m_lineNumber = in_lineNumber;
  errorData.m_columnNumber = in_columnNumber;
  m_errorData.PushBack(errorData);
}

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
