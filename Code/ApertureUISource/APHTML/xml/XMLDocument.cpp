#include <APCore/Interfaces/APCFileSystem.h>
#include <APHTML/dom/DOMAttribute.h>
#include <APHTML/dom/DOMCDataSection.h>
#include <APHTML/dom/DOMComment.h>
#include <APHTML/dom/DOMDocument.h>
#include <APHTML/dom/DOMElement.h>
#include <APHTML/dom/DOMNode.h>
#include <APHTML/xml/XMLDocument.h>

template <typename ElementType>
bool aperture::xml::XMLDocument<ElementType>::LoadDocument(const nsDynamicArray<nsUInt8>& xml_buffer)
{
  nsUInt64 xmlsize = sizeof(xml_buffer.GetData());
  if (m_internaldocument->load_buffer(xml_buffer.GetData(), xmlsize) == false)
  {
    return false;
  }
  return true;
}
template <typename ElementType>
bool aperture::xml::XMLDocument<ElementType>::LoadDocument(nsString& xml_file)
{
  if (m_internaldocument->load_file(xml_file) == false)
  {
    return false;
  }
  return true;
}

template <typename ElementType>
void aperture::xml::XMLDocument<ElementType>::ComposeDocumentTree()
{
  /// GOAL: Iterate through the XML document for every node type, and store them within the internal document tree, so other Modules can access them.
  for (auto& node : m_internaldocument->children())
  {
    switch (node.type())
    {
      case pugi::node_cdata:
        /// Create CData DOM Object.
        aperture::dom::DOMCDataSection CDataSection;
        CDataSection.data_ = node.value();
        if (node == m_internaldocument->_root)
        {
          this->InternalDocumentTree->set_head(CDataSection);
        }
        else
        {
          /// If the node is not the root node, then we need to add it to the tree, by finding its parent node.
          if (auto sibiling = this->InternalDocumentTree.sibling(CDataSection) != nullptr)
          {
            sibiling->append_child(CDataSection);
          }
          else
          {
            /// NOTE: This should be hit if the child does not have a proper parent. we can then assume its a DOCTYPE Descriptor, and just add it to the tree.
            this->InternalDocumentTree->sibling(this->InternalDocumentTree.head)->append_child(CDataSection);
          }
        }
        break;
      case pugi::node_element:
        // TODO: This is a local object, that will get freed, upon resolution. this is unsafe, and should be resolved!
        /// Create Element DOM Object.
        aperture::dom::DOMElement Element;
        Element.getNodeName() = node.name();
        if (node == m_internaldocument->_root)
        {
          this->InternalDocumentTree->set_head(Element);
        }
        else
        {
          /// If the node is not the root node, then we need to add it to the tree, by finding its parent node.
          if (auto sibiling = this->InternalDocumentTree.sibling(Element) != nullptr)
          {
            sibiling->append_child(Element);
          }
          else
          {
            /// NOTE: This should be hit if the child does not have a proper parent. we can then assume its a DOCTYPE Descriptor, and just add it to the tree.
            this->InternalDocumentTree->sibling(this->InternalDocumentTree.head)->append_child(Element);
          }
        }
        break;
      case pugi::node_pcdata:
        // TODO: Implement PCDATA DOM Object.
        NS_ASSERT_NOT_IMPLEMENTED;
        break;
      case pugi::node_pi:
        // TODO: Implement Processing Instruction DOM Object.
        NS_ASSERT_NOT_IMPLEMENTED;
        break;
      case pugi::node_comment:
        /// Create Comment DOM Object.
        aperture::dom::DOMComment Comment;
        Comment.content = node.value();
        if (node == m_internaldocument->_root)
        {
          this->InternalDocumentTree->set_head(Comment);
        }
        else
        {
          /// If the node is not the root node, then we need to add it to the tree, by finding its parent node.
          if (auto sibiling = this->InternalDocumentTree.sibling(Comment) != nullptr)
          {
            sibiling->append_child(Comment);
          }
          else
          {
            /// NOTE: This should be hit if the child does not have a proper parent. we can then assume its a DOCTYPE Descriptor, and just add it to the tree.
            this->InternalDocumentTree->sibling(this->InternalDocumentTree.head)->append_child(Comment);
          }
        }
        break;
      case pugi::node_doctype:
        // TODO: Implement DOCTYPE DOM Object.
        NS_ASSERT_NOT_IMPLEMENTED;
        break;
      case pugi::node_declaration:
        // TODO: Implement Declaration DOM Object.
        NS_ASSERT_NOT_IMPLEMENTED;
        break;
      case pugi::node_null:
        break;
      case pugi::node_document:
        /// Create Document DOM Object.
        aperture::dom::DOMDocument Document;
        if (node == m_internaldocument->_root)
        {
          Document.m_documentpath = static_cast<const char*>(node.name());
          this->InternalDocumentTree->set_head(Document);
        }
        else
        {
          /// If the node is not the root node, then we need to add it to the tree, by finding its parent node.
          if (auto sibiling = this->InternalDocumentTree.sibling(Document) != nullptr)
          {
            sibiling->append_child(Document);
          }
          else
          {
            /// NOTE: This should be hit if the child does not have a proper parent. we can then assume its a DOCTYPE Descriptor, and just add it to the tree.
            this->InternalDocumentTree->sibling(this->InternalDocumentTree.head)->append_child(Document);
          }
        }
        break;
    };
  }
}

template <typename ElementType>
nsUInt64 aperture::xml::XMLDocument<ElementType>::GetNodeCount()
{
  for (auto& node : m_internaldocument->children())
  {
    this->NodeCount++;
  }
  return this->NodeCount;
}

template <typename ElementType>
nsUInt64 aperture::xml::XMLDocument<ElementType>::GetElementCount()
{
  int value = 0;
  for (auto& node : m_internaldocument->children())
  {
    if (node.type() == pugi::node_element)
    {
      value++;
    }
  }
  return value;
}

template <typename ElementType>
nsUInt64 aperture::xml::XMLDocument<ElementType>::GetAttributeCount()
{
  int value = 0;
  for (auto& node : m_internaldocument->children())
  {
    for (auto& attribute : node.attributes())
    {
      value++;
    }
  }
  return value;
}
