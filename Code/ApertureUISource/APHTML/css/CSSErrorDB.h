/*
 *   Copyright (c) 2024 WD Studios L.L.C.
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */
#pragma once

#include <APHTML/APEngineCommonIncludes.h>

namespace aperture::css
{
  struct NS_APERTURE_DLL CSSParsedFileData
  {
  public:
    std::size_t m_fileSize;
    /// @brief Absolute or relative (or VFS) file path to the CSS file.
    nsStringView m_filePath;
  };

  class NS_APERTURE_DLL CSSErrorDatabase
  {
    NS_DECLARE_SINGLETON(CSSErrorDatabase);
  public:
    
    friend class css_logger_error_handler;

    CSSErrorDatabase() : m_SingletonRegistrar(this) {}

    enum CSSErrorType
    {
      CSS_ERROR_SYNTAX,      // Syntax Error.
      CSS_ERROR_SEMANTIC,    // Semantic Error.
      CSS_ERROR_COMPOSITION, // Composition Error, such as recursive variable resolution.
      CSS_ERROR_UNKNOWN      // Unknown Error.
    };

    struct CSSErrorData
    {
    public:
      CSSErrorType m_errorType;
      CSSParsedFileData m_fileData;
      nsString m_errorMessage;
      int m_lineNumber;
      int m_columnNumber;
    };

    // Adds an error to the database with detailed information.
    void AddError(CSSErrorType in_errorType, const CSSParsedFileData& in_fileData, const std::string& in_errorMessage, int in_lineNumber, int in_columnNumber)
    {
      m_errorData.PushBack(CSSErrorData{in_errorType, in_fileData, in_errorMessage.c_str(), in_lineNumber, in_columnNumber});
    }

    // Clears all stored errors.
    void ClearErrors()
    {
      m_errorData.Clear();
    }

    // Prints all errors in a user-friendly format.
    void PrintErrors()
    {
      for (const auto& error : m_errorData)
      {
        nsLog::Info("CSS Error: {0}\nFile: {1}\nLine: {2}\nColumn: {3}", error.m_errorMessage.GetData(), error.m_fileData.m_filePath.GetStartPointer(), error.m_lineNumber, error.m_columnNumber);
      }
    }

    void PrintErrorsUnstructured()
    {
      for (const auto& error : m_errorDataRaw)
      {
        nsLog::Info("CSS Error: {0}", error);
      }
    }

  private:
    nsDynamicArray<CSSErrorData> m_errorData;
    nsDynamicArray<const char*> m_errorDataRaw;
    // Converts error type to a string representation for user-friendly output.
    const char* ErrorTypeToString(CSSErrorType type) const
    {
      switch (type)
      {
        case CSS_ERROR_SYNTAX:
          return "Syntax Error";
        case CSS_ERROR_SEMANTIC:
          return "Semantic Error";
        case CSS_ERROR_COMPOSITION:
          return "Composition Error";
        case CSS_ERROR_UNKNOWN:
          return "Unknown Error";
        default:
          return "Invalid Error Type";
      }
    }
  };
} // namespace aperture::css
