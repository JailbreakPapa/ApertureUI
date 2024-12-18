#include <APCore/Interfaces/APCPlatform.h>
#include <APHTML/css/functions/CSSVarFunction.h>
#include <APHTML/core/CSSController.h>
#include <APCore/Interfaces/APCUtils.h>

void aperture::css::CSSVarFunction::EvaluateVariable(const char* in_variable, CSSVarVariant& out_var)
{
  nsString str = in_variable;
  css::CSSParsedFileData fileData;
  // First check the characters of the variable. this can tell us what type of variable it is most of the time.
  // Checking the first 3 characters for a variable function. We reject the variable due to the risk of a recursive loop(needing to resolve the variable again, again, again).
  // NOTE: The only exception to this rule is if we are within existing function (e.g. calc, min, max, etc...)
  if ((const char*)str[0, 2] == "var")
  {
    aperture::CSSController::GetErrorDatabase().AddError(CSSErrorDatabase::CSSErrorType::CSS_ERROR_COMPOSITION,fileData,core::Format("CSSVarFunction::EvaluateVariable: Recursive loop detected. Rejecting variable. {0}", in_variable).GetData(),);
    // return since we can't resolve a recursive variable.
    return;
  }
  else if ((const char*)str[0, 2] == "rgb" || (const char*)str[0, 3] == "rgba" || (const char*)str[0, 2] == "hsl" || (const char*)str[0, 3] == "hsla")
  {
    // We have a color variable.
    out_var.m_internalvariant =  nsVariant::Type::Color;
    // We are now done. the parser should now handle the rest of the work.
    // TODO: Should we return a object of the now found function?
  }
  else if ((const char*)str[0, 3] == "url")
  {
    // We have a url variable.
    out_var = nsVariant::Type::URL;
  }
  else if ((const char*)str[0, 5] == "linear-gradient" || (const char*)str[0, 6] == "radial-gradient" || (const char*)str[0, 6] == "conic-gradient")
  {
    // We have a gradient variable.
    out_var = nsVariant::Type::Gradient;
  }
  else if ((const char*)str[0, 8] == "repeating-linear-gradient" || (const char*)str[0, 9] == "repeating-radial-gradient" || (const char*)str[0, 9] == "repeating-conic-gradient")
  {
    // We have a repeating gradient variable.
    out_var = nsVariant::Type::Gradient;
  }
  else if ((const char*)str[0, 12] == "cubic-bezier")
  {
    // We have a cubic bezier variable.
    out_var = nsVariant::Type::Bezier;
  }
  else
  {
    // We have a number variable.
    out_var = nsVariant::Type::Number;
  }
}