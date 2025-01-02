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
#include <Foundation/Math/Math.h>

namespace aperture::core
{
  struct AnsiAllocator
  {
    struct AnyType
    {
    };

    template <size_t kSizeOfElement>
    struct AnyTypeAllocator
    {
      AnyTypeAllocator()
        : data(nullptr)
        , num(0)
      {
      }

      ~AnyTypeAllocator()
      {
        if (data)
        {
          nsFoundation::GetDefaultAllocator()->Deallocate(data);
        }
      }

      AnyTypeAllocator(AnyTypeAllocator&& other) noexcept
      {
        data = other.data;
        num = other.num;
        other.data = nullptr;
        other.num = 0;
      }

      AnyTypeAllocator& operator=(AnyTypeAllocator&& other) noexcept
      {
        data = other.data;
        num = other.num;
        other.data = nullptr;
        other.num = 0;
        return *this;
      }

      AnyTypeAllocator(const AnyTypeAllocator& other) = delete;
      AnyTypeAllocator& operator=(const AnyTypeAllocator& other) = delete;

      void resize(size_t p_last_num, size_t p_num)
      {
        data = (AnyType*)nsFoundation::GetDefaultAllocator()->Allocate((std::size_t)data, nsMath::Pow2((int)(p_num * kSizeOfElement)));
        NS_ISE_JSI_CHECK(data);
        const size_t added_count = p_num - p_last_num;
        memset((void*)((unsigned char*)data + p_last_num * kSizeOfElement), 0,
          added_count * kSizeOfElement);
        num = p_num;
      }

      AnyType* get_data() const
      {
        return data;
      }

      size_t capacity() const { return num; }

      AnyType* data;
      size_t num;
    };

    template <typename T>
    struct ForType : AnyTypeAllocator<sizeof(T)>
    {
      T* get_data() const
      {
        return (T*)AnyTypeAllocator<sizeof(T)>::get_data();
      }
    };
  };
} // namespace aperture::core
