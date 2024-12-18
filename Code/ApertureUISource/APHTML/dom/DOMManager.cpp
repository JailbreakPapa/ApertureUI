#include <APHTML/dom/DOMAttribute.h>
#include <APHTML/dom/DOMElement.h>
#include <APHTML/dom/DOMAttribute.h>

#include <APHTML/dom/DOMElement.h>
#include "DOMManager.h"

aperture::dom::DOMElement aperture::dom::DOMManager::CreateElement(const nsString& in_tagname)
{
  aperture::dom::DOMElement newelement(in_tagname);
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
  for (int i = 0; i < DOMElementArray.GetCount(); i++)
  {
    acollection.push_back(DOMElementArray[i]);
  }
  collection.buildTree(acollection);
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
