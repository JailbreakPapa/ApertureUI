/*
 *   Copyright (c) 2024 WD Studios L.L.C.
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */
#pragma once

#include <APHTML/css/syntax/CSSSyntaxProperties.h>

namespace aperture::css
{
  // @brief Internal Interface for a CSSFunction.
  // @details use this class to implement the basic functionality of a CSS Function.
  class NS_APERTURE_DLL CSSIFunction
  {
  public:
    CSSIFunction() = default;

    explicit CSSIFunction(const char* in_functionname)
      : m_functionname(in_functionname)
    {
    }

    virtual ~CSSIFunction() = default;

    virtual void SetFunctionPosition(int in_line, int in_column) = 0;

  protected:
    virtual void RegisterFunction(const char* in_function, bool is_evaluatable) = 0;

  private:
    nsUInt32 m_line;
    nsUInt32 m_column;
    const char* m_functionname;
  };
} // namespace aperture::css
