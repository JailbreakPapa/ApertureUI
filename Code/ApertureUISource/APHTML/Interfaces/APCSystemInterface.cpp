#include "APCSystemInterface.h"

aperture::core::IAPCSystemInterface::IAPCSystemInterface()
{
}

aperture::core::IAPCSystemInterface::~IAPCSystemInterface()
{
}

double aperture::core::IAPCSystemInterface::GetElapsedTime()
{
  return 0.0;
}

int aperture::core::IAPCSystemInterface::TranslateString(nsString& translated, const nsString& input)
{
  return 0;
}

void aperture::core::IAPCSystemInterface::SetMouseCursor(const nsString& cursor_name)
{
}

void aperture::core::IAPCSystemInterface::SetClipboardText(const nsString& text)
{
}

void aperture::core::IAPCSystemInterface::GetClipboardText(nsString& text)
{
}

void aperture::core::IAPCSystemInterface::ActivateKeyboard(nsVec2 caret_position, float line_height)
{
}

void aperture::core::IAPCSystemInterface::DeactivateKeyboard()
{
}
