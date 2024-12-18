#include <APHTML/dom/DOMNode.h>
#include <string>
#include <vector>
/// NOTE: The DLL/PCH Header should always be included last.
#include <APHTML/APEngineDLL.h>
namespace aperture::dom
{
  /**
   * @class DOMComment
   * @brief Represents a comment node in the Document Object Model (DOM).
   *
   * The DOMComment class provides a representation of a comment node in the DOM.
   * It can be used to create, modify, and access comment nodes in an HTML document.
   */
  class NS_APERTURE_DLL DOMComment
  {
  public:
    /**
     * @brief Constructs a DOMComment object with the specified text content.
     * @param textContent The text content of the comment node.
     */
    DOMComment(const std::string& textContent)
      : type(DOMNodeType::TEXT_NODE)
      , content(textContent)
    {
    }


    /**
     * @brief Gets the type of the comment node.
     * @return The type of the comment node.
     */
    DOMNodeType getType() const { return type; }

    /**
     * @brief Gets the content of the comment node.
     * @return The content of the comment node.
     */
    const std::string& getContent() const { return content; }

    /**
     * @brief Gets the tag name of the comment node.
     * @return The tag name of the comment node.
     */
    const std::string& getTagName() const { return tagName; }

    /**
     * @brief Gets the children of the comment node.
     * @return The children of the comment node.
     */
    const std::vector<DOMComment*>& getChildren() const { return children; }

    /**
     * @brief Adds a child comment node to the comment node.
     * @param child The child comment node to add.
     */
    void addChild(DOMComment* child) { children.push_back(child); }

    // Additional methods (optional)
    void removeChild(DOMComment* child) {
      children.emplace_back(child);
      children.pop_back();
    }
    void setTextContent(const std::string& text) { content = text; }

  private:
    DOMNodeType type;                  // The type of the comment node
    std::string content;               // The content of the comment node (for text nodes)
    std::string tagName;               // The tag name of the comment node (for element nodes)
    std::vector<DOMComment*> children; // The children of the comment node
  };
} // namespace aperture::dom
