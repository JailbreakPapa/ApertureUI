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

#include <APHTML/APEngineCommonIncludes.h>
#include <APHTML/Interfaces/Internal/APCBuffer.h>


namespace aperture::dom
{
  /*
   * @brief DOMEvent is a internal DOM representation of a Nodes Events.
   *
   * @note This is somewhat compliant with W3C DOM specification, providing methods for accessing and manipulating event nodes in the DOM tree.
   * @see See https://www.w3.org/TR/uievents/ for reference.
   */
  class NS_APERTURE_DLL DOMEvent : public nsReflectedClass
  {
    NS_ALLOW_PRIVATE_PROPERTIES(aperture::dom::DOMEvent);

  public:
    enum class Phase
    {
      NONE,
      CAPTURING_PHASE,
      AT_TARGET,
      BUBBLING_PHASE
    };

    explicit DOMEvent(const char* in_eventname, bool bubbles = false, bool cancelable = false)
      : type_(in_eventname)
      , bubbles_(bubbles)
      , cancelable_(cancelable)
      , defaultPrevented_(false)
      , propagationStopped_(false)
      , eventPhase_(Phase::NONE)
    {
    }

    // Accessors
    std::string type() const { return type_; }
    bool bubbles() const { return bubbles_; }
    bool cancelable() const { return cancelable_; }
    bool isDefaultPrevented() const { return defaultPrevented_; }
    Phase eventPhase() const { return eventPhase_; }

    // Methods
    void stopPropagation() { propagationStopped_ = true; }
    void preventDefault()
    {
      if (cancelable_)
      {
        defaultPrevented_ = true;
      }
    }
    bool isPropagationStopped() const { return propagationStopped_; }

    // For internal use during event dispatch
    void setEventPhase(Phase phase) { eventPhase_ = phase; }

  private:
    std::string type_;
    bool bubbles_;
    bool cancelable_;
    bool defaultPrevented_;
    bool propagationStopped_;
    Phase eventPhase_;
  };

  /*
   * @brief DOMEvents are (well, should be) dispatched to DOMEventTargets when an event occurs.
   * Its entirely Dependant on the user to dispatch events, if done from native code.
   *
   * Custom events should be the one that dispatches events (e.g. level change event, and you want the engine to clean up the GC, etc.)
   * Otherwise, any DOMNode will handle this for you.
   *
   * @note Events are naturally aysnc as well.
  */
  class NS_APERTURE_DLL DOMEventTarget
  {
  public:
    using EventListener = std::function<void(DOMEvent&)>;

    void addEventListener(const std::string& type, EventListener listener)
    {
      listeners_[type].push_back(listener);
    }

    void dispatchEvent(DOMEvent& event)
    {
      event.setEventPhase(DOMEvent::Phase::AT_TARGET);

      auto& handlers = listeners_[event.type()];
      for (auto& handler : handlers)
      {
        if (!event.isPropagationStopped())
        {
          handler(event);
        }
      }
    }

  private:
    std::unordered_map<std::string, std::vector<EventListener>> listeners_;
  };
} // namespace aperture::dom
