/*
 *   Copyright (c) 2024 WD Studios L.L.C.
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */
#pragma once
#include <APHTML/APEngineCommonIncludes.h>
#include <APHTML/css/SpiritIncludes.h>
#include <APHTML/core/CSSController.h>
namespace aperture::css::spirit
{
    using standard_iterator = std::string::const_iterator;
    using foundation_interation = nsStringIterator;

    namespace x3 = boost::spirit::x3;

    struct NS_APERTURE_DLL CSSErrorHandler
    {
        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(Iterator& first, Iterator const& last, Exception const& x, Context const& context)
        {
            auto& error_handler = x3::get<x3::error_handler_tag>(context).get();
            core::IAPCPlatform::GetLoggingSystem()->LogError("CSS Error: %s\nFile: %s\nLine: %d\nColumn: %d", x.what(), error_handler.fileData.m_filePath, error_handler.lineNumber, error_handler.columnNumber);
            CSSErrorDatabase::AddError(error_handler.fileData, x.what(), error_handler.lineNumber, error_handler.columnNumber);
            return x3::error_handler_result::fail;
        }
    };

}