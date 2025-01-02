#include "DOMCollection.h"

namespace aperture::dom
{

  DOMCollection::DOMCollection(const std::vector<std::shared_ptr<DOMElement>>& elements)
  {
    buildTree(elements);
  }

  const std::vector<std::shared_ptr<DOMElement>>& DOMCollection::getRootElements() const
  {
    return m_rootElements;
  }

  std::shared_ptr<DOMElement> DOMCollection::getElementByIndex(int index) const
  {
    auto it = m_indexMap.find(index);
    return (it != m_indexMap.end()) ? it->second : nullptr;
  }

  std::shared_ptr<DOMElement> DOMCollection::getParentElement(const DOMElement* element) const
  {
    auto it = m_parentMap.find(element);
    return (it != m_parentMap.end()) ? it->second : nullptr;
  }

  void DOMCollection::appendElement(const std::shared_ptr<DOMElement>& element)
  {
    if (!element)
    {
      return;
    }

    // If no parent exists, treat it as a root element.
    if (element->getParentNode().get() == nullptr)
    {
      m_rootElements.push_back(element);
    }
    // Update ID map if the element has an ID.
    const std::string& id = element->getId();
    if (!id.empty())
    {
      m_idMap[id] = element;
    }

    // Update index map.
    m_indexMap[element->getIndex()] = element;
  }

  void DOMCollection::removeElement(const DOMElement* element)
  {
    if (!element)
    {
      return;
    }

    // Remove from root elements if it's a root.
    m_rootElements.erase(std::remove_if(m_rootElements.begin(), m_rootElements.end(),
                           [element](const std::shared_ptr<DOMElement>& root)
                           {
                             return root.get() == element;
                           }),
      m_rootElements.end());

    // Remove from ID map.
    auto itId = m_idMap.find(element->getId());
    if (itId != m_idMap.end())
    {
      m_idMap.erase(itId);
    }

    // Remove from index map.
    auto itIndex = m_indexMap.find(element->getIndex());
    if (itIndex != m_indexMap.end())
    {
      m_indexMap.erase(itIndex);
    }

    // Remove from parent map.
    m_parentMap.erase(element);
  }

  void DOMCollection::buildTree(const std::vector<std::shared_ptr<DOMElement>>& elements)
  {
    m_rootElements.clear();
    m_indexMap.clear();
    m_idMap.clear();
    m_parentMap.clear();

    for (const auto& element : elements)
    {
      appendElement(element);

      if (element->getParentElement())
      {
        setParentChildRelation(element->getParentElement(), element);
      }
      else
      {
        m_rootElements.push_back(element);
      }
    }
  }

  std::shared_ptr<DOMElement> DOMCollection::getElementById(const std::string& id) const
  {
    auto it = m_idMap.find(id);
    return (it != m_idMap.end()) ? it->second : nullptr;
  }

  void DOMCollection::setParentChildRelation(const std::shared_ptr<DOMElement>& parent, const std::shared_ptr<DOMElement>& child)
  {
    m_parentMap[child.get()] = parent;

    if (parent)
    {
      parent->appendChild(child);
    }
  }

} // namespace aperture::dom
