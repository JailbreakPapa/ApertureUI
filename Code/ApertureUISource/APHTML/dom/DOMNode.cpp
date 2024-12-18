#include <APHTML/core/BaseDocument.h>
#include <APHTML/dom/DOMNode.h>

using namespace aperture::dom;

// Implementation

DOMNode::DOMNode(aperture::dom::DOMNodeType nodeType, const std::string &nodeName)
    : m_nodeType(nodeType), m_nodeName(nodeName) {}

aperture::dom::DOMNodeType DOMNode::getNodeType() const {
    return m_nodeType;
}

const std::string &DOMNode::getNodeName() const {
    return m_nodeName;
}

const std::string &DOMNode::getNodeValue() const {
    return m_nodeValue;
}

std::shared_ptr<DOMNode> DOMNode::getParentNode() const {
    return m_parentNode.lock();
}

const std::vector<std::shared_ptr<DOMNode>> &DOMNode::getChildNodes() const {
    return m_childNodes;
}

std::shared_ptr<DOMNode> DOMNode::getFirstChild() const {
    return m_childNodes.empty() ? nullptr : m_childNodes.front();
}

std::shared_ptr<DOMNode> DOMNode::getLastChild() const {
    return m_childNodes.empty() ? nullptr : m_childNodes.back();
}

std::shared_ptr<DOMNode> DOMNode::getPreviousSibling() const {
    if (auto parent = getParentNode()) {
        auto &siblings = parent->getChildNodes();
        auto it = std::find(siblings.begin(), siblings.end(), this);
        return (it != siblings.begin() && it != siblings.end()) ? *(it - 1) : nullptr;
    }
    return nullptr;
}

std::shared_ptr<DOMNode> DOMNode::getNextSibling() const {
    if (auto parent = getParentNode()) {
        auto &siblings = parent->getChildNodes();
        auto it = std::find(siblings.begin(), siblings.end(), this);
        return (it != siblings.end() && it + 1 != siblings.end()) ? *(it + 1) : nullptr;
    }
    return nullptr;
}

void DOMNode::setNodeValue(const std::string &value) {
    m_nodeValue = value;
}

bool DOMNode::hasChildNodes() const {
    return !m_childNodes.empty();
}

void DOMNode::appendChild(const std::shared_ptr<DOMNode> &newChild) {
    if (newChild) {
        m_childNodes.push_back(newChild);
        newChild->m_parentNode = m_parentNode;
    }
}

void DOMNode::removeChild(const std::shared_ptr<DOMNode> &child) {
    if (!child) {
        return;
    }
    m_childNodes.erase(std::remove(m_childNodes.begin(), m_childNodes.end(), child), m_childNodes.end());
    child->m_parentNode.reset();
}

void DOMNode::insertBefore(const std::shared_ptr<DOMNode> &newChild, const std::shared_ptr<DOMNode> &refChild) {
    if (newChild && refChild) {
        auto it = std::find(m_childNodes.begin(), m_childNodes.end(), refChild);
        if (it != m_childNodes.end()) {
            m_childNodes.insert(it, newChild);
            newChild->m_parentNode = m_parentNode;
        }
    }
}

