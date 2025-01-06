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
#include <APHTML/V8Engine/V8EngineDLL.h>
#include <functional>
#include <type_traits>
#include <unordered_map>
#include <mutex>
#include <v8.h>

namespace aperture::v8::binding
{
  /// @brief A class for binding C++ functions and variables to JS.
  /// @warning This class is not thread-safe, due to the nature of Isolates. You can use this natively, but its recommended to just directly use the EngineRuntime to Bind Everything.
  class NS_V8ENGINE_DLL V8EBinder
  {
  public:
    explicit V8EBinder(::v8::Isolate* isolate)
      : isolate_(isolate)
    {
        global_template_.Reset(isolate, ::v8::ObjectTemplate::New(isolate));
    }

    template <typename ReturnType, typename... Args>
    void BindFunctionToJS(const char* name, ReturnType (*func)(Args...));

    void BindFunctionToJS(const char* name, const std::function<void()>& func);

    template <typename T>
    void BindVariable(const char* name, const T& value);

    template <typename ClassType>
    void BindClass(const char* name);

    template <typename ClassType, typename BaseType>
    void InheritBoundClass();

    template <typename ClassType, typename MemberType>
    void BindClassVariable(const char*className, const char* memberName, MemberType ClassType::*member);

    template <typename ClassType, typename ReturnType, typename... Args>
    void BindClassFunction(const char* className, const std::string& functionName, ReturnType (ClassType::*func)(Args...));

    ::v8::Local<::v8::Object> CreateJSModule(const char* name);

    void SetIsolate(::v8::Isolate* isolate);

    ::v8::Isolate* GetIsolate() const;

  private:
    std::mutex mutex_;
    ::v8::Isolate* isolate_;
    ::v8::Global<::v8::ObjectTemplate> global_template_;
    std::unordered_map<std::string, ::v8::Global<::v8::FunctionTemplate>> class_templates_;
  };
} // namespace aperture::v8::binding
