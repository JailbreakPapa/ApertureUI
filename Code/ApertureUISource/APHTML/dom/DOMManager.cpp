#include <APHTML/dom/DOMAttribute.h>
#include <APHTML/dom/DOMElement.h>

#include "DOMManager.h"

aperture::dom::DOMElement aperture::dom::DOMManager::CreateElement(const nsString& in_tagname)
{
  aperture::dom::DOMElement newelement(in_tagname.GetData());
  // TODO: Why are we returning local objects....
  DOMElementArray.PushBack(newelement);
  return newelement;
}

aperture::dom::DOMElement aperture::dom::DOMManager::GetCurrentActedUponElement() const
{
  return DOMElementArray[DOMElementArray.GetCount() - 1];
}

bool aperture::dom::DOMManager::operator<(const DOMManager& rhs) const
{
  return DOMElementArray.GetCount() < rhs.DOMElementArray.GetCount() && m_iterationele < rhs.m_iterationele;
}

void aperture::dom::DOMManager::SerializeDOMCollection()
{
  std::vector<std::shared_ptr<aperture::dom::DOMElement>> sharedAcollection;
  sharedAcollection.reserve(DOMElementArray.GetCount());

  for (size_t i = 0; i < DOMElementArray.GetCount(); i++)
  {
    sharedAcollection.push_back(std::make_shared<aperture::dom::DOMElement>(DOMElementArray[static_cast<nsUInt32>(i)]));
  }

  collection.buildTree(sharedAcollection);
}

aperture::dom::DOMManager::DOMManager()
{
  GlobalDOMManager = this;
}

aperture::dom::DOMManager::~DOMManager()
{
  DOMElementArray.Clear();
  delete GlobalDOMManager;
}

void aperture::dom::DOMManager::SetCurrentActedUponElement(const aperture::dom::DOMElement& in_element)
{
  DOMElementArray.PushBack(in_element);
}

