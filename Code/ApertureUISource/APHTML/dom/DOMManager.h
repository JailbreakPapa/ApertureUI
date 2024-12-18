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

#include <APHTML/dom/DOMInformation.h>
#include <Foundation/Containers/DynamicArray.h>
#include <Foundation/Containers/HybridArray.h>
#include <Foundation/Containers/List.h>
#include <Foundation/Containers/Map.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/Types/UniquePtr.h>
#include <APHTML/dom/DOMCollection.h>
/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>

namespace aperture::dom
{
  class DOMElement;
  class DOMAttribute;
  class DOMNode;
  class NS_APERTURE_DLL DOMManager : public nsReflectedClass
  {
    NS_ALLOW_PRIVATE_PROPERTIES(aperture::dom::DOMManager);

  public:
    virtual ~DOMManager();

    /**
     * Creates a new DOMElement with the specified tag name.
     *
     * @param in_tagname The tag name of the element to create.
     * @return A pointer to the newly created DOMElement.
     */
    DOMElement CreateElement(const nsString& in_tagname);
    DOMElement GetCurrentActedUponElement() const;
    bool operator<(const DOMManager& rhs) const;
  private:
    /// <summary>
    /// Serializes & Caches DOMCollection(s), along with the files. this should be the "database" of all constructed layouts.
    /// </summary>
    void SerializeDOMCollection();

    DOMManager();

    void SetCurrentActedUponElement(const aperture::dom::DOMElement& in_element);
    
    /**
     * @brief An array of DOMElement objects. The collection of all DOM elements. we use this to grab the current acted upon element.
     *
     * This array stores multiple instances of the DOMElement class.
     * It is used in the DOMManager class to manage and manipulate DOM elements.
     */
    nsDynamicArray<DOMElement> DOMElementArray;
    std::vector<aperture::dom::DOMElement> acollection;
    DOMCollection collection{acollection};
    int m_iterationele = 0;
  };
} // namespace aperture::dom

/// @note Don't define within the name spaces!
NS_DECLARE_REFLECTABLE_TYPE(NS_APERTURE_DLL, aperture::dom::DOMManager);

inline static aperture::dom::DOMManager* GlobalDOMManager;
// Path: DOMElement.h
