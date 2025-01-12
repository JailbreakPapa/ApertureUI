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
#include <APHTML/APEngineDLL.h>
#include <Foundation/Containers/StaticArray.h>
#include <Foundation/Logging/Log.h>
#include <Foundation/Types/Delegate.h>
#include <Foundation/Containers/Implementation/ArrayIterator.h>

namespace aperture::core
{

  /// @brief A Agnostic buffer that can be resized and copied into. Mainly used for File I/O.
  template <typename T>
  class CoreBuffer
  {
  protected:
    T* m_data;
    size_t m_size;

  public:
    using const_iterator = const T*;
    using const_reverse_iterator = const_reverse_pointer_iterator<T>;

  public:
    CoreBuffer(nsDynamicArray<T> in_Content, size_t initialSize = (size_t)128 * 128 * 1024);
    CoreBuffer(void* in_Data, size_t in_Size, size_t initialSize = (size_t)128 * 128 * 1024);
    CoreBuffer(const char* r_data, size_t initialSize = (size_t)128 * 128 * 1024);

    explicit CoreBuffer(size_t initialSize = (size_t)128 * 128 * 1024);

    NS_ADD_DEFAULT_OPERATOR_NOTEQUAL(const CoreBuffer<T>&);

    // @brief The Righthand side of the operator is the data to copy. this will forcefully copy the data into the buffer and resize it.
    void operator=(T* r_data);

    T& operator[](T* r_data);

    T& operator[](nsUInt32 index);
    
    ~CoreBuffer();

    void resize(size_t newSize);

    T* get(size_t size);

    size_t size() const;

    // All of the iterator stuff
    CoreBuffer<T>::const_interator begin() const { return m_data; }
    CoreBuffer<T>::const_interator end() const { return m_data + m_size; }
    CoreBuffer<T>::const_reverse_interator rbegin() const { return m_data + m_size - 1; }
    CoreBuffer<T>::const_reverse_interator rend() const { return m_data - 1; }
    CoreBuffer<T>::const_interator cbegin() const { return m_data; }
    CoreBuffer<T>::const_interator cend() const { return m_data + m_size; }
  };

  template <typename T>
  inline size_t aperture::core::CoreBuffer<T>::size() const
  {
    return m_size;
  }

  template <typename T>
  inline T* aperture::core::CoreBuffer<T>::get(size_t size)
  {
    if(size == 0)
    {
      size = m_size;
    }
    if (size > m_size)
      resize(size);

    return m_data;
  }

  template <typename T>
  inline void CoreBuffer<T>::operator=(T* r_data)
  {
    m_data = r_data;
    resize(sizeof(r_data));
  }

  template <typename T>
  inline T& CoreBuffer<T>::operator[](T* r_data)
  {
    return m_data[r_data];
  }

  template <typename T>
  inline T& CoreBuffer<T>::operator[](nsUInt32 index)
  {
    return static_cast<const T*>(this)->get()[uiIndex];
  }

  template <typename T>
  inline aperture::core::CoreBuffer<T>::~CoreBuffer()
  {
    if (m_data != nullptr)
      delete[] m_data;
  }

  template <typename T>
  inline CoreBuffer<T>::CoreBuffer(nsDynamicArray<T> in_Content, size_t initialSize)
  {
    in_Content.Compact();
    m_data = in_Content.GetElementsPtr();
    m_size = sizeof(in_Content);
  }

  template <typename T>
  inline CoreBuffer<T>::CoreBuffer(void* in_Data, size_t in_Size, size_t initialSize)
  {
    m_data = in_Data;
    m_size = in_Size;
  }

  template <typename T>
  inline CoreBuffer<T>::CoreBuffer(const char* r_data, size_t initialSize)
  {
    m_data = r_data;
    m_size = sizeof(r_data);
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
