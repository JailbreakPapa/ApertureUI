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
            /// @brief Hard/Memory Internal File path. this may be scuffed
            nsStringView m_filePath;
    };
    // TODO: Refactor this class.
    class NS_APERTURE_DLL CSSErrorDatabase
    {
        public:
            enum CSSErrorType
            {
                CSS_ERROR_SYNTAX, // Syntax Error.
                CSS_ERROR_SEMANTIC, // Semantic Error.
                CSS_ERROR_COMPOSITION, // Composition Error. This could mean that a recursive path was found (e.g. when resolving a variable that references itself).
                CSS_ERROR_UNKNOWN
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
        public:
            void AddError(CSSErrorType in_errortype,CSSParsedFileData& in_fileData, const char* in_errorMessage, int in_lineNumber, int in_columnNumber);
            void ClearErrors();
            void PrintErrors();
        private:
            nsDynamicArray<CSSErrorData> m_errorData;
    };
}
