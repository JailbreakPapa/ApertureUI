/*
 *   Copyright (c) 2024 WD Studios L.L.C.
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */
#pragma once
#include <Foundation/Basics.h>
#include <Foundation/Types/Variant.h>
namespace aperture::css
{
    /// @brief Enum class for CSS Standards.
    /// Aperture UI does NOT enforce CSS Standard (apart from CSS3) compliance as of now. this will come in a later release.
    enum class CSSStandard
    {
        CSS3,
        CSS2,
        CSS1,
    };
    NS_DECLARE_POD_TYPE(CSSStandard);
}