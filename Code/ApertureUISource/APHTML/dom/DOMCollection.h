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

#include <APHTML/dom/DOMElement.h>
#include <Foundation/Containers/HybridArray.h>
#include <Foundation/Containers/List.h>
#include <Foundation/Containers/Map.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/Types/UniquePtr.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>

namespace aperture::dom {

/**
 * @brief DOMCollection represents a virtual DOM tree for a document. It provides methods for
 * constructing, traversing, and manipulating the document's tree structure.
 */
class NS_APERTURE_DLL DOMCollection {
public:
     DOMCollection() = default;
    /**
     * @brief Constructs a DOMCollection object with the given elements.
     * @param elements The vector of DOMElements to be used for constructing the DOMCollection.
     */
    DOMCollection(const std::vector<std::shared_ptr<DOMElement>> &elements);

    /**
     * @brief Gets the root elements of the DOMCollection.
     * @return A const reference to the vector of shared pointers to DOMElements representing the root elements.
     */
    const std::vector<std::shared_ptr<DOMElement>> &getRootElements() const;

    /**
     * @brief Gets the DOMElement at the specified index.
     * @param index The index of the DOMElement to retrieve.
     * @return A shared pointer to the DOMElement at the specified index, or nullptr if the index is out of range.
     */
    std::shared_ptr<DOMElement> getElementByIndex(int index) const;

    /**
     * @brief Gets the parent DOMElement of the specified DOMElement.
     * @param element A pointer to the DOMElement for which to find the parent.
     * @return A shared pointer to the parent DOMElement, or nullptr if the specified element is not found or has no parent.
     */
    std::shared_ptr<DOMElement> getParentElement(const DOMElement *element) const;

    /**
     * @brief Appends a new DOMElement to the tree.
     * @param element The new DOMElement to be appended.
     */
    void appendElement(const std::shared_ptr<DOMElement> &element);

    /**
     * @brief Removes a DOMElement from the tree.
     * @param element The DOMElement to be removed.
     */
    void removeElement(const DOMElement *element);

    /**
     * @brief Builds the tree structure of the DOMCollection using the given elements.
     * @param elements The vector of DOMElements to be used for building the tree.
     */
    void buildTree(const std::vector<std::shared_ptr<DOMElement>> &elements);

    /**
     * @brief Finds a DOMElement by its ID attribute.
     * @param id The ID of the DOMElement to find.
     * @return A shared pointer to the DOMElement with the specified ID, or nullptr if not found.
     */
    std::shared_ptr<DOMElement> getElementById(const std::string &id) const;

private:
    std::vector<std::shared_ptr<DOMElement>> m_rootElements; ///< The root elements of the DOMCollection.
    std::unordered_map<int, std::shared_ptr<DOMElement>> m_indexMap; ///< A map of element indices to their corresponding DOMElements.
    std::unordered_map<std::string, std::shared_ptr<DOMElement>> m_idMap; ///< A map of element IDs to their corresponding DOMElements.
    std::unordered_map<const DOMElement *, std::shared_ptr<DOMElement>> m_parentMap; ///< A map of child elements to their parent elements.

    /**
     * @brief Updates the parent-child relationships for the tree.
     * @param parent The parent DOMElement.
     * @param child The child DOMElement.
     */
    void setParentChildRelation(const std::shared_ptr<DOMElement> &parent, const std::shared_ptr<DOMElement> &child);
};

} // namespace aperture::dom

/// @note Don't define within the namespaces!
NS_DECLARE_REFLECTABLE_TYPE(NS_APERTURE_DLL, aperture::dom::DOMCollection);
