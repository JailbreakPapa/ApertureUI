/*
This code is part of Aperture UI - A HTML/CSS/JS UI Middleware

Copyright (c) 2020-2024 WD Studios L.L.C. and/or its licensors. All
rights reserved in all media.

The coded instructions, statements, computer programs, and/or related
material (collectively the "Data") in these files contain confidential
and unpublished information proprietary WD Studios and/or its
licensors, which is protected by United States of America federal
copyright law and by international treaties.

This software or source code is supplied under the terms of a license
agreement and nondisclosure agreement with WD Studios L.L.C. and may
not be copied, disclosed, or exploited except in accordance with the
terms of that agreement. The Data may not be disclosed or distributed to
third parties, in whole or in part, without the prior written consent of
WD Studios L.L.C..

WD STUDIOS MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS
SOURCE CODE FOR ANY PURPOSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER, ITS AFFILIATES,
PARENT COMPANIES, LICENSORS, SUPPLIERS, OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OR PERFORMANCE OF THIS SOFTWARE OR SOURCE CODE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once

#include <APHTML/DOM/events/DOMEvent.h>

namespace aperture::dom
{
  /*
   * @brief DOMKeyboardEvent is a internal DOM representation of a Keyboard Event.
  */
  class DOMKeyboardEvent : public DOMEvent
  {
  public:
    DOMKeyboardEvent(const char* type, const std::string& key, const std::string& code,
      bool altKey = false, bool ctrlKey = false, bool shiftKey = false, bool metaKey = false,
      bool bubbles = false, bool cancelable = false)
      : DOMEvent(type, bubbles, cancelable)
      , key_(key)
      , code_(code)
      , altKey_(altKey)
      , ctrlKey_(ctrlKey)
      , shiftKey_(shiftKey)
      , metaKey_(metaKey)
    {
    }

    // Accessors
    std::string key() const { return key_; }
    std::string code() const { return code_; }
    bool altKey() const { return altKey_; }
    bool ctrlKey() const { return ctrlKey_; }
    bool shiftKey() const { return shiftKey_; }
    bool metaKey() const { return metaKey_; }

  private:
    std::string key_;  // The key value
    std::string code_; // The physical key on the keyboard
    bool altKey_;      // Whether the Alt key was pressed
    bool ctrlKey_;     // Whether the Ctrl key was pressed
    bool shiftKey_;    // Whether the Shift key was pressed
    bool metaKey_;     // Whether the Meta (Command/Windows) key was pressed
  };
} // namespace aperture::dom
