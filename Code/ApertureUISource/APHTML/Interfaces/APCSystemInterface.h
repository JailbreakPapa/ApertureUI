/*
 *   Copyright (c) 2023 Watch Dogs LLC
 *   All rights reserved.
 *   You are only allowed access to this code, if given WRITTEN permission by WD Studios L.L.C.
 */

#pragma once

#include <APHTML/APEngineDLL.h>
namespace aperture::core
{
  /*
    System interface provides an interface for time, translation, logging, and other system utilities.
    The default logging implementation outputs to the Windows Debug Console on Windows, and Standard Error on other
    platforms.
  */
  class NS_APERTURE_DLL IAPCSystemInterface
  {
  public:
    IAPCSystemInterface();
    virtual ~IAPCSystemInterface();

    /// Get the number of seconds elapsed since initalization of APUI.
    /// @return Elapsed time, in seconds.
    virtual double GetElapsedTime();

    /// Translate the input string into the translated string.
    /// @param[out] translated Translated string ready for display.
    /// @param[in] input nsString as received from the DOM.
    /// @return Number of translations that occurred.
    virtual int TranslateString(nsString& translated, const nsString& input);

    /// Set mouse cursor.
    /// @param[in] cursor_name Cursor name to activate.
    virtual void SetMouseCursor(const nsString& cursor_name);

    /// Set clipboard text.
    /// @param[in] text Text to apply to clipboard.
    virtual void SetClipboardText(const nsString& text);

    /// Get clipboard text.
    /// @param[out] text Retrieved text from clipboard.
    virtual void GetClipboardText(nsString& text);

    /// Activate keyboard (for touchscreen devices).
    /// @param[in] caret_position Position of the caret in absolute window coordinates.
    /// @param[in] line_height Height of the current line being edited.
    virtual void ActivateKeyboard(nsVec2 caret_position, float line_height);

    /// Deactivate keyboard (for touchscreen devices).
    virtual void DeactivateKeyboard();
  };
} // namespace aperture::core
