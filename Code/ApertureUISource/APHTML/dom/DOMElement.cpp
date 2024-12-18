#include "DOMElement.h"
#include "DOMAttribute.h"
#include "DOMManager.h"

using namespace aperture::dom;

DOMElement::DOMElement(const nsString& tagName, int index)
  : m_elementname(tagName)
  , m_index(index)
{
}

void aperture::dom::DOMElement::EmplaceClass(const nsString& in_class_name)
{
  m_elementclass = in_class_name;
}

void aperture::dom::DOMElement::EmplaceName(const nsString& in_name)
{
  m_elementname = in_name;
}

aperture::dom::DOMElement aperture::dom::DOMElement::GetParentElement()
{
  return parent();
}

bool aperture::dom::DOMElement::operator==(const aperture::dom::DOMElement& r) const
{
  return m_elementname == r.m_elementname && m_elementclass == r.m_elementclass;
}
bool aperture::dom::DOMElement::operator!=(const aperture::dom::DOMElement& r) const
{
  return m_elementname != r.m_elementname;
}

bool aperture::dom::DOMElement::operator<(const aperture::dom::DOMElement& r) const
{
  if (m_elementname < r.m_elementname && m_elementclass < r.m_elementclass && m_elementvalue < r.m_elementvalue && m_type < r.m_type)
  {
    for (auto& a : atrributes)
    {
      for (auto& b : r.atrributes)
      {
        if (a < b)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
    }
  }
  else
  {
    return false;
  }
}

bool aperture::dom::DOMElement::operator>(const aperture::dom::DOMElement& r) const
{
  if (m_elementname > r.m_elementname && m_elementclass > r.m_elementclass && m_elementvalue > r.m_elementvalue && m_type > r.m_type)
  {
    for (auto a : atrributes)
    {
      for (auto b : r.atrributes)
      {
        if (a > b)
        {
          return true;
        }
        else
        {
          return false;
        }
      }
    }
  }
  else
  {
    return false;
  }
}

bool aperture::dom::DOMElement::operator<=(const aperture::dom::DOMElement& r) const
{
  return !(*this > r);
}

bool aperture::dom::DOMElement::operator>=(const aperture::dom::DOMElement& r) const
{
  return !(*this < r);
}

bool aperture::dom::DOMElement::empty() const
{
  return m_elementname == nullptr;
}

aperture::dom::DOMNodeType aperture::dom::DOMElement::type() const
{
  return this->m_type;
}

const nsString DOMElement::name() const
{
  return (const nsString)m_elementname;
}

const nsString aperture::dom::DOMElement::value() const
{
  return (const nsString)m_elementvalue;
}

aperture::dom::DOMElement aperture::dom::DOMElement::parent() const
{
  // TODO: Check for the very root element!
  return GlobalDOMManager->GetCurrentActedUponElement(); // This is a placeholder
}

void aperture::dom::DOMElement::setAttribute(const dom::DOMAttribute& name, const nsString& value)
{
  atrributes[atrributes.IndexOf(name)].SetAttrValue(value);
}

int aperture::dom::DOMElement::getIndex() const
{
  return m_index;
}
