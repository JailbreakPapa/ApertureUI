#include "DOMElement.h"
#include "DOMAttribute.h"
#include "DOMManager.h"
#include <memory>

using namespace aperture::dom;

DOMElement::DOMElement(const std::string& tagName)
  : DOMNode(DOMNodeType::ELEMENT_NODE, tagName)
  , m_tagName(tagName)
{
}

const std::string& DOMElement::getTagName() const
{
  return m_tagName;
}

std::string DOMElement::getAttribute(const std::string& name) const
{
  auto it = m_attributes.find(name);
  return it != m_attributes.end() ? it->second : "";
}

void DOMElement::setAttribute(const std::string& name, const std::string& value)
{
  m_attributes[name] = value;
}

void DOMElement::removeAttribute(const std::string& name)
{
  m_attributes.erase(name);
}

std::vector<std::shared_ptr<DOMElement>> DOMElement::getElementsByTagName(const std::string& tagName) const
{
  std::vector<std::shared_ptr<DOMElement>> elements;
  for (const auto& child : m_children)
  {
    if (auto element = std::dynamic_pointer_cast<DOMElement>(child))
    {
      if (element->getTagName() == tagName)
      {
        elements.push_back(element);
      }
      auto childElements = element->getElementsByTagName(tagName);
      elements.insert(elements.end(), childElements.begin(), childElements.end());
    }
  }
  return elements;
}

void DOMElement::appendChild(const std::shared_ptr<DOMNode>& child)
{
  if (child)
  {
    m_children.push_back(child);
    if (auto element = std::dynamic_pointer_cast<DOMElement>(child))
    {
      element->m_parent = std::dynamic_pointer_cast<DOMElement>(shared_from_this());
    }
  }
}

void DOMElement::removeChild(const std::shared_ptr<DOMNode>& child)
{
  m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
  if (auto element = std::dynamic_pointer_cast<DOMElement>(child))
  {
    element->m_parent.reset();
  }
}

std::shared_ptr<DOMElement> DOMElement::getParentElement() const
{
  return m_parent.lock();
}
