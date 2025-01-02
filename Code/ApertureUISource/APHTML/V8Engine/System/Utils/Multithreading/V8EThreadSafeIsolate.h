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

#include <APHTML/Interfaces/APCPlatform.h>
#include <Foundation/Threading/LockedObject.h>
#include <Foundation/Threading/Mutex.h>
#include <APHTML/V8Engine/V8EngineDLL.h>

namespace aperture::v8
{
  // @brief "Thread Safe" Access to a Isolate. this should be created per Script Thread, due to access rules.
  class NS_V8ENGINE_DLL V8EThreadSafeIsolate
  {
  public:
    virtual ~V8EThreadSafeIsolate() = default;

    /// @brief Creates the internal isolate object. This Function REQUIRES you to create this on the Main Thread.
    /// @param params custom options
    /// @return If assigning and creating the isolate was successful.
    bool CreateIsolate(::v8::Isolate::CreateParams& params);

    NS_ALWAYS_INLINE ::v8::Isolate* AccessInternalIsolate()
    {
      if (mangaged_isolate != nullptr)
      {
        return mangaged_isolate;
      }
      else
      {
        nsLog::Error("The Isolate Provided Is nullptr. Did you initialize it correctly?");
        return nullptr;
      }
    }

  private:
    nsMutex mangaged_isolate_mutex;
    ::v8::Isolate* mangaged_isolate;
  };
} // namespace aperture::v8
