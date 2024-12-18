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
#include <APHTML/dom/DOMAttribute.h>
#include <APHTML/dom/DOMNode.h>
#include <Foundation/Containers/DynamicArray.h>
#include <Foundation/Containers/HybridArray.h>
#include <Foundation/Containers/List.h>
#include <Foundation/Containers/Map.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/Types/UniquePtr.h>
#include <memory>
#include <string>
#include <vector>
/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>
#include <unordered_map>

namespace aperture::dom
{
  /**
   * @brief The DOMElement class represents an element node in the Document Object Model (DOM).
   *
   * This class is compliant with the W3C DOM specification, providing methods for accessing and
   * manipulating element nodes in the DOM tree.
   */
  class NS_APERTURE_DLL DOMElement : public DOMNode
  {
  public:
    /**
     * @brief Constructs a DOMElement object with the specified tag name.
     *
     * @param tagName The name of the tag for the DOM element.
     */
    explicit DOMElement(const std::string& tagName);

    /**
     * @brief Gets the tag name of the element.
     *
     * @return A constant reference to the tag name of the element.
     */
    const std::string& getTagName() const;

    /**
     * @brief Retrieves the value of an attribute by name.
     *
     * @param name The name of the attribute.
     * @return The value of the attribute, or an empty string if the attribute does not exist.
     */
    std::string getAttribute(const std::string& name) const;

    /**
     * @brief Sets an attribute on the element.
     *
     * @param name The name of the attribute.
     * @param value The value to set for the attribute.
     */
    void setAttribute(const std::string& name, const std::string& value);

    /**
     * @brief Removes an attribute from the element.
     *
     * @param name The name of the attribute to remove.
     */
    void removeAttribute(const std::string& name);

    /**
     * @brief Gets a list of child elements with the specified tag name.
     *
     * @param tagName The name of the tag to match.
     * @return A vector of shared pointers to child elements with the matching tag name.
     */
    std::vector<std::shared_ptr<DOMElement>> getElementsByTagName(const std::string& tagName) const;

    /**
     * @brief Adds a child node to the element.
     *
     * @param child A shared pointer to the child node to add.
     */
    void appendChild(const std::shared_ptr<DOMNode>& child);

    /**
     * @brief Removes a child node from the element.
     *
     * @param child A shared pointer to the child node to remove.
     */
    void removeChild(const std::shared_ptr<DOMNode>& child);

    /**
     * @brief Gets the parent element of this element.
     *
     * @return A shared pointer to the parent element, or nullptr if the element has no parent.
     */
    std::shared_ptr<DOMElement> getParentElement() const;

  private:
    std::string m_tagName;                                     ///< The tag name of the element.
    std::unordered_map<std::string, std::string> m_attributes; ///< The attributes of the element.
    std::vector<std::shared_ptr<DOMNode>> m_children;          ///< The child nodes of the element.
    std::weak_ptr<DOMElement> m_parent;                        ///< The parent element.
  };
} // namespace aperture::dom
