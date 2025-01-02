/*
 *   Copyright (c) 2024 WD Studios L.L.C.
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */
#pragma once
#include <APHTML/APEngineCommonIncludes.h>

namespace aperture::css
{
  // TODO: Implement CSSVarVariant
  class NS_APERTURE_DLL CSSVarVariant
  {
  public:
    enum class UniqueType
    {
      Number,
      URL,
      Gradient,
      Bezier,
      Color,
    };
    UniqueType m_type = UniqueType::URL;
    nsVariant m_internalvariant;
  };
} // namespace aperture::css
