/*
 *   Copyright (c) 2024 WD Studios L.L.C.
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */
#pragma once

#include <APHTML/css/functions/CSSVarVariant.h>
#include <APHTML/css/functions/CSSIFunction.h>

namespace aperture::css
{
  class NS_APERTURE_DLL CSSVarFunction : public CSSIFunction
  {
  public:
    virtual void RegisterFunction(const char* in_function, bool is_evaluatable) override;

    static void EvaluateVariable(const char* in_variable, CSSVarVariant& out_var);
  private:
    CSSVarVariant m_var;
  };
} // namespace aperture::css