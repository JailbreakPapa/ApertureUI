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
#include <APHTML/Interfaces/Internal/APCBuffer.h>
#include <Foundation/Containers/HybridArray.h>
#include <Foundation/Containers/List.h>
#include <Foundation/Containers/Map.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/Types/UniquePtr.h>

/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>

namespace aperture::dom
{
  class DOMElement;
  /// @brief DOMAttribute is a internal DOM representation of a Nodes Attributes.
  class NS_APERTURE_DLL DOMAttribute : public nsReflectedClass
  {
    NS_ALLOW_PRIVATE_PROPERTIES(aperture::dom::DOMAttribute);

  public:
    explicit DOMAttribute(const nsString& in_attrname, const nsVariant& in_attrvalue);
    /**
     * Sets the attribute name.
     *
     * @param in_newname The new name for the attribute.
     */
    void SetAttrName(const nsString& in_newname);
    /**
     * Sets the attribute value.
     *
     * @param in_newvalue The new value to set for the attribute.
     */
    void SetAttrValue(const nsVariant& in_newvalue);
    nsVariantType::Enum GetAttrType() { return m_attrtype; }

    // Comparison operators (compares wrapped node pointers)
    bool operator==(const aperture::dom::DOMAttribute& r) const;
    bool operator!=(const aperture::dom::DOMAttribute& r) const;
    bool operator<(const aperture::dom::DOMAttribute& r) const;
    bool operator>(const aperture::dom::DOMAttribute& r) const;
    bool operator<=(const aperture::dom::DOMAttribute& r) const;
    bool operator>=(const aperture::dom::DOMAttribute& r) const;

  private:
    /// <summary>
    /// Checks out what value the attr is given and decides if its a string,function,etc...
    /// </summary>
    /// <param name="in_newvalue">Value to Deduce.</param>
    void IndexAttrValueType(const nsVariant& in_newvalue);

  private:
    nsVariantType::Enum m_attrtype;
    const char* m_attrname;
    nsVariant m_attrvalue;
  };
} // namespace aperture::dom

/// @note Don't define within the name spaces!
NS_DECLARE_REFLECTABLE_TYPE(NS_APERTURE_DLL, aperture::dom::DOMAttribute);
