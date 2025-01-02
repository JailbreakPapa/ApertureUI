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

#include <APHTML/V8Engine/V8EngineDLL.h>

namespace aperture::v8
{
    struct JavaScriptTimerAction
    {
        NS_FORCE_INLINE JavaScriptTimerAction() = default;

        NS_FORCE_INLINE JavaScriptTimerAction(::v8::Global<::v8::Function>&& p_func, int p_argc) : function_(std::move(p_func)), argc_(p_argc)
        {
            if (p_argc > 0)
            {
                //NOTE unsafe
                argv_ = (::v8::Global<::v8::Value>*) nsFoundation::GetDefaultAllocator()->Allocate(sizeof(::v8::Global<::v8::Value>) * p_argc,0);
                memset((void*) argv_, 0, sizeof(::v8::Global<::v8::Value>) * p_argc);
            }
            else
            {
                argv_ = nullptr;
            }
        }

        NS_FORCE_INLINE ~JavaScriptTimerAction()
        {
            if (argc_ > 0)
            {
                //NOTE unsafe
                for (int index = 0 ; index < argc_; ++index)
                {
                    argv_[index].Reset();
                }
                nsFoundation::GetDefaultAllocator()->Deallocate(argv_);
            }
            function_.Reset();
        }

        NS_FORCE_INLINE JavaScriptTimerAction(JavaScriptTimerAction&& p_other) noexcept
            : function_(std::move(p_other.function_)), argc_(p_other.argc_), argv_(p_other.argv_)
        {
            p_other.argc_ = 0;
        }

        NS_FORCE_INLINE JavaScriptTimerAction& operator=(JavaScriptTimerAction&& p_other) noexcept
        {
            function_ = std::move(p_other.function_);
            argc_ = p_other.argc_;
            argv_ = p_other.argv_;
            p_other.argc_ = 0;
            return *this;
        }

        NS_FORCE_INLINE bool operator!() const { return function_.IsEmpty(); }

        NS_FORCE_INLINE void store(int index, ::v8::Global<::v8::Value>&& value)
        {
            NS_ISE_JSI_CHECK(index >= 0 && index < argc_);
            argv_[index] = std::move(value);
        }

        void operator()(::v8::Isolate* isolate);

    private:
        ::v8::Global<::v8::Function> function_;
        int argc_;
        ::v8::Global<::v8::Value>* argv_;
    };
}
