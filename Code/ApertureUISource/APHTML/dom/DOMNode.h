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

#include <Foundation/Containers/HybridArray.h>
#include <iostream>
#include <memory>
#include <Foundation/Containers/List.h>
#include <Foundation/Containers/Map.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/Types/UniquePtr.h>

/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>

namespace aperture::dom
{
  /// @brief Type of node enum.
  enum class NS_APERTURE_DLL DOMNodeType : nsUInt8
  {
    ELEMENT_NODE = 1,
    ATTRIBUTE_NODE = 2,
    TEXT_NODE = 3,
    CDATA_SECTION_NODE = 4,
    PROCESSING_INSTRUCTION_NODE = 5,
    COMMENT_NODE = 6,
    DOCUMENT_NODE = 7,
    DOCUMENT_TYPE_NODE = 8,
    DOCUMENT_FRAGMENT_NODE = 9
  };
  /**
   * @brief The DOMNode class represents a node in the Document Object Model (DOM).
   *
   * The DOMNode class provides functionality to manipulate and traverse nodes in the DOM tree.
   * It defines various types of nodes and provides methods to access and modify them.
   *
   * @note This class is part of the aperture::dom namespace.
   */
  class NS_APERTURE_DLL DOMNode : public nsReflectedClass
  {
    NS_ALLOW_PRIVATE_PROPERTIES(aperture::dom::DOMNode);

  public:
    // Constructors and Destructor
    explicit DOMNode(DOMNodeType nodeType, const std::string& nodeName);
    DOMNode() = default;
    virtual ~DOMNode() = default;

    // Getters
    DOMNodeType getNodeType() const;
    const std::string& getNodeName() const;
    const std::string& getNodeValue() const;
    std::shared_ptr<DOMNode> getParentNode() const;
    const std::vector<std::shared_ptr<DOMNode>>& getChildNodes() const;
    std::shared_ptr<DOMNode> getFirstChild() const;
    std::shared_ptr<DOMNode> getLastChild() const;
    std::shared_ptr<DOMNode> getPreviousSibling() const;
    std::shared_ptr<DOMNode> getNextSibling() const;

    // Setters
    void setNodeValue(const std::string& value);

    // Methods
    bool hasChildNodes() const;
    void appendChild(const std::shared_ptr<DOMNode>& newChild);
    void removeChild(const std::shared_ptr<DOMNode>& child);
    void insertBefore(const std::shared_ptr<DOMNode>& newChild, const std::shared_ptr<DOMNode>& refChild);

  protected:
    DOMNodeType m_nodeType;
    std::string m_nodeName;
    std::string m_nodeValue;

    std::weak_ptr<DOMNode> m_parentNode;
    std::vector<std::shared_ptr<DOMNode>> m_childNodes;
  public:
    
  bool operator==(const DOMNode& other) const;
  };
  /// @brief NodeList.
} // namespace aperture::dom


/// @note Don't define within the name spaces!
NS_DECLARE_REFLECTABLE_TYPE(NS_APERTURE_DLL, aperture::dom::DOMNode);
