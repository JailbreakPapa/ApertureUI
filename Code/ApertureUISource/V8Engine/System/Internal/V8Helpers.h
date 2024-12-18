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
#include <APCore/Interfaces/Internal/APCBuffer.h>
#include <V8Engine/System/Internal/V8EObjectHandle.h>


namespace aperture::v8::helpers
{
  inline namespace
  {
    static nsString stringify(::v8::Isolate* isolate, const ::v8::Local<::v8::Context>& context, const ::v8::Local<::v8::Value>& p_val)
    {
      // TODO:
      return "";
    }

    // convert int64 to int32 anyway
    NS_FORCE_INLINE static ::v8::Local<::v8::Integer> to_int32(::v8::Isolate* isolate, int64_t p_val)
    {
      return ::v8::Int32::New(isolate, (int32_t)p_val);
    }

    static int32_t to_int32(const ::v8::Local<::v8::Context>& context, ::v8::MaybeLocal<::v8::Value> obj, const int32_t p_default_value)
    {
      int32_t value;
      ::v8::Local<::v8::Value> local;
      if (obj.ToLocal(&local) && local->Int32Value(context).To(&value))
        return value;
      return p_default_value;
    }

    template <typename TEnum>
    static TEnum to_enum(const ::v8::Local<::v8::Context>& context, ::v8::MaybeLocal<::v8::Value> obj, const TEnum p_default_value)
    {
      int32_t value;
      ::v8::Local<::v8::Value> local;
      if (obj.ToLocal(&local) && local->Int32Value(context).To(&value))
        return (TEnum)value;
      return p_default_value;
    }

    NS_FORCE_INLINE static ::v8::Local<::v8::Boolean> to_boolean(::v8::Isolate* isolate, bool p_val)
    {
      return ::v8::Boolean::New(isolate, p_val);
    }

    NS_FORCE_INLINE static ::v8::Local<::v8::Number> to_number(::v8::Isolate* isolate, double p_val)
    {
      return ::v8::Number::New(isolate, p_val);
    }

    /**
     * @brief Convert a v8 utf-16 string to a WD Studios String
     */
    NS_FORCE_INLINE static nsString to_string(const ::v8::String::Value& p_val)
    {
      nsString str;
      str = (const char*)*p_val;
      return str;
    }

    /**
     * Convert a javascript 'nsString/Symbol' value to a WD Studios String
     */
    static nsString to_string(::v8::Isolate* isolate, const ::v8::Local<::v8::Value>& p_val)
    {
      nsString str;
      if (!p_val.IsEmpty() && (p_val->IsString() || p_val->IsSymbol()))
      {
        if (const ::v8::String::Utf8Value exchange(isolate, p_val); *exchange)
        {
          str = *exchange;
        }
      }
      return str;
    }

    NS_FORCE_INLINE static nsString to_string_opt(::v8::Isolate* isolate, const ::v8::MaybeLocal<::v8::Value>& p_val)
    {
      ::v8::Local<::v8::Value> local;
      return p_val.ToLocal(&local) ? to_string(isolate, local) : nsString();
    }



    static nsString to_string_without_side_effect(::v8::Isolate* isolate, const ::v8::Local<::v8::Value>& p_val)
    {
      nsString str;
      if (!p_val.IsEmpty() && (p_val->IsString() || p_val->IsSymbol()))
      {
        ::v8::Local<::v8::String> detail_string;
        if (p_val->ToDetailString(isolate->GetCurrentContext()).ToLocal(&detail_string))
        {
          if (const ::v8::String::Utf8Value exchange(isolate, detail_string); *exchange)
          {
            str = *exchange;
          }
        }
      }
      return str;
    }

    template <size_t N>
    NS_FORCE_INLINE static ::v8::Local<::v8::String> to_string(::v8::Isolate* isolate, const char (&literal)[N])
    {
      return ::v8::String::NewFromUtf8Literal(isolate, literal, ::v8::NewStringType::kNormal);
    }

    NS_FORCE_INLINE static ::v8::Local<::v8::String> to_string(::v8::Isolate* isolate, const nsString& p_str)
    {
      const nsString cstr = p_str;
      return ::v8::String::NewFromUtf8(isolate, cstr, ::v8::NewStringType::kNormal, cstr.GetCharacterCount()).ToLocalChecked();
    }

    NS_FORCE_INLINE static ::v8::Local<::v8::String> to_string_ascii(::v8::Isolate* isolate, const nsString& p_str)
    {
      const nsString cstr = p_str;
      return ::v8::String::NewFromOneByte(isolate, (const uint8_t*)cstr.GetData(), ::v8::NewStringType::kNormal, cstr.GetCharacterCount()).ToLocalChecked();
    }
    template <typename T = nsUInt8>
    static core::CoreBuffer<T> to_core_buffer(::v8::Isolate* isolate, const ::v8::Local<::v8::ArrayBuffer>& array_buffer)
    {
      const size_t size = array_buffer->ByteLength();
      core::CoreBuffer packed;
      nsResult err = packed.resize((int)size);
      NS_ISE_JSI_CHECK(err == NS_SUCCESS);
      const void* data = array_buffer->Data();
      memcpy(packed.get(), data, size);
      return packed;
    }

    static ::v8::Local<::v8::ArrayBuffer> to_array_buffer(::v8::Isolate* isolate, const nsDynamicArray<uint8_t>& packed)
    {
      ::v8::Local<::v8::ArrayBuffer> buffer = ::v8::ArrayBuffer::New(isolate, sizeof(packed.GetData()));
      void* data = buffer->Data();
      memcpy(data, packed.GetData(), sizeof(packed.GetData()));
      return buffer;
    }


    static ::v8::MaybeLocal<::v8::Script> compile(::v8::Local<::v8::Context> context, ::v8::Local<::v8::String> source, const nsString& p_filename)
    {
      ::v8::Isolate* isolate = context->GetIsolate();
      if (p_filename.IsEmpty())
      {
        return ::v8::Script::Compile(context, source);
      }

#if NS_ENABLED(NS_PLATFORM_WINDOWS_DESKTOP)
      nsStringBuilder filename = p_filename;
      filename.ReplaceAll(nsStringView("/"), nsStringView("\\"));
#else
      const nsString filename = p_filename;
#endif
      ::v8::ScriptOrigin origin(isolate, ::v8::String::NewFromUtf8(isolate, filename, ::v8::NewStringType::kNormal, filename.GetCharacterCount()).ToLocalChecked());
      return ::v8::Script::Compile(context, source, &origin);
    }
  } // namespace
}; // namespace aperture::v8::helpers
