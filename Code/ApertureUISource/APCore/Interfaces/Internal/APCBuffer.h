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
#include <APCore/COREAPEngineDLL.h>
#include <Foundation/Containers/StaticArray.h>
#include <Foundation/Logging/Log.h>
#include <Foundation/Types/Delegate.h>

namespace aperture::core
{
  /// NOTE: DEPRECATED DONT USE: Use CoreBuffer Instead.
  using APCBuffer = nsDynamicArray<nsUInt8>;

  template <typename T>
  class CoreBuffer
  {
  protected:
    T* m_data;
    size_t m_size;

  public:
    explicit CoreBuffer(size_t initialSize = (size_t)128 * 128 * 1024);
    // @brief The Righthand side of the operator is the data to copy. this will forcefully copy the data into the buffer and resize it.
    void operator=(const char* r_data);
    ~CoreBuffer();

    void resize(size_t newSize);

    T* get(size_t size = size());

    size_t size() const;
  };

  template <typename T>
  inline size_t aperture::core::CoreBuffer<T>::size() const
  {
    return m_size;
  }

  template <typename T>
  inline T* aperture::core::CoreBuffer<T>::get(size_t size)
  {
    if (size > m_size)
      resize(size);

    return m_data;
  }

  template <typename T>
  inline void CoreBuffer<T>::operator=(const char* r_data)
  {
    m_data = r_data;
    resize(sizeof(r_data));
  }

  template <typename T>
  inline aperture::core::CoreBuffer<T>::~CoreBuffer()
  {
    if (m_data != nullptr)
      delete[] m_data;
  }

  template <typename T>
  inline aperture::core::CoreBuffer<T>::CoreBuffer(size_t initialSize)
    : m_data(nullptr)
    , m_size(0)
  {
    resize(initialSize);
  }

  template <typename T>
  inline void aperture::core::CoreBuffer<T>::resize(size_t newSize)
  {
    if (m_data != nullptr)
      delete[] m_data;

    m_data = new T[newSize];
    m_size = newSize;
  }
} // namespace aperture::core
