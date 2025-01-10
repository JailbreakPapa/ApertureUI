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

#include <APHTML/APEngineCommonIncludes.h>

namespace aperture::dom
{
  // Represents a node in the DOM Prefix Tree.
  class NS_APERTURE_DLL DOMPrefixTreeNode
  {
  public:
    DOMPrefixTreeNode(const std::string& name)
      : tagName(name)
      , isEndOfTag(false)
    {
    }

    // Adds a child node or retrieves an existing one.
    std::shared_ptr<DOMPrefixTreeNode> addOrGetChild(const std::string& name)
    {
      if (!children.count(name))
      {
        children[name] = std::make_shared<DOMPrefixTreeNode>(name);
      }
      return children[name];
    }

    // Searches for a child node by name.
    std::shared_ptr<DOMPrefixTreeNode> getChild(const std::string& name) const
    {
      auto it = children.find(name);
      return it != children.end() ? it->second : nullptr;
    }

    // Returns all child nodes.
    const std::unordered_map<std::string, std::shared_ptr<DOMPrefixTreeNode>>& getChildren() const
    {
      return children;
    }

    // Sets the node as the end of a valid tag.
    void setEndOfTag(bool isEnd)
    {
      isEndOfTag = isEnd;
    }

    // Checks if this node marks the end of a tag.
    bool isEnd() const
    {
      return isEndOfTag;
    }

    // Returns the tag name for this node.
    const std::string& getTagName() const
    {
      return tagName;
    }

  private:
    std::string tagName;
    bool isEndOfTag;
    std::unordered_map<std::string, std::shared_ptr<DOMPrefixTreeNode>> children;
  };

  // The DOM Prefix Tree class.
  class NS_APERTURE_DLL DOMPrefixTree
  {
  public:
    DOMPrefixTree()
    {
      root = std::make_shared<DOMPrefixTreeNode>("");
    }

    // Inserts a tag path into the tree.
    void insert(const std::vector<std::string>& tagPath)
    {
      auto current = root;
      for (const auto& tag : tagPath)
      {
        current = current->addOrGetChild(tag);
      }
      current->setEndOfTag(true);
    }

    // Checks if a full tag path exists in the tree.
    bool contains(const std::vector<std::string>& tagPath) const
    {
      auto current = root;
      for (const auto& tag : tagPath)
      {
        current = current->getChild(tag);
        if (!current)
        {
          return false;
        }
      }
      return current->isEnd();
    }

    // Prints all valid tag paths stored in the tree.
    void printAllTags() const
    {
      std::vector<std::string> currentPath;
      printAllTagsHelper(root, currentPath);
    }

  private:
    std::shared_ptr<DOMPrefixTreeNode> root;

    // Helper function for printing all tags recursively.
    void printAllTagsHelper(std::shared_ptr<DOMPrefixTreeNode> node, std::vector<std::string>& currentPath) const
    {
      if (node->isEnd())
      {
        for (const auto& tag : currentPath)
        {
            nsLog::Info(tag.c_str());
        }
      }

      for (const auto& [tagName, childNode] : node->getChildren())
      {
        currentPath.push_back(tagName);
        printAllTagsHelper(childNode, currentPath);
        currentPath.pop_back();
      }
    }
  };

} // namespace aperture::dom