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
#include <APHTML/layout/Core/LayoutNode.h>
#include <vector>
#include <yoga/Yoga.h>

namespace aperture::layout
{
  class NS_APERTURE_DLL BlockContainer : public LayoutNode
  {
  public:
    BlockContainer(const Style& style)
      : LayoutNode(style)
    {
      node = YGNodeNew();
      YGNodeStyleSetFlexDirection(node, YGFlexDirectionColumn);
      YGNodeStyleSetDisplay(node, YGDisplayFlex);
    }

    BlockContainer()
    {
      node = YGNodeNew();
      YGNodeStyleSetFlexDirection(node, YGFlexDirectionColumn);
      YGNodeStyleSetDisplay(node, YGDisplayFlex);
    }

    ~BlockContainer()
    {
      YGNodeFreeRecursive(node);
    }

    void addChild(std::shared_ptr<LayoutNode> child) override
    {
      auto blockChild = std::dynamic_pointer_cast<BlockContainer>(child);
      if (blockChild)
      {
        children_.push_back(child);
        YGNodeInsertChild(node, blockChild->getYogaNode(), YGNodeGetChildCount(node));
      }
      else
      {
        children_.push_back(child);
      }
    }

    void calculateLayout(float width, float height) override
    {
      YGNodeCalculateLayout(node, width, height, YGDirectionLTR);
      size_ = {width, height};
      positionChildren();
    }

  protected:
    void calculateRowLayout(const Size& availableSpace) override
    {
      float totalWidth = 0.0f;
      for (auto& child : children_)
      {
        totalWidth += child->size_.width;
      }

      float remainingWidth = availableSpace.width - totalWidth;
      float offsetX = 0.0f;

      for (auto& child : children_)
      {
        child->position_.x = offsetX;
        child->position_.y = 0.0f;
        offsetX += child->size_.width + (remainingWidth / children_.size());
      }
    }

    void calculateColumnLayout(const Size& availableSpace) override
    {
      float totalHeight = 0.0f;
      for (auto& child : children_)
      {
        totalHeight += child->size_.height;
      }

      float remainingHeight = availableSpace.height - totalHeight;
      float offsetY = 0.0f;

      for (auto& child : children_)
      {
        child->position_.x = 0.0f;
        child->position_.y = offsetY;
        offsetY += child->size_.height + (remainingHeight / children_.size());
      }
    }

    void positionChildren() override
    {
      if (YGNodeStyleGetFlexDirection(node) == YGFlexDirectionRow)
      {
        calculateRowLayout(size_);
      }
      else
      {
        calculateColumnLayout(size_);
      }

      for (uint32_t i = 0; i < YGNodeGetChildCount(node); ++i)
      {
        auto childNode = YGNodeGetChild(node, i);
        float left = YGNodeLayoutGetLeft(childNode);
        float top = YGNodeLayoutGetTop(childNode);
        float width = YGNodeLayoutGetWidth(childNode);
        float height = YGNodeLayoutGetHeight(childNode);

        if (i < children_.size())
        {
          auto& child = children_[i];
          child->position_ = {left, top};
          child->size_ = {width, height};
        }
      }
    }

  public:
    void setSize(float width, float height)
    {
      YGNodeStyleSetWidth(node, width);
      YGNodeStyleSetHeight(node, height);
    }

    void setMargin(YGEdge edge, float value)
    {
      YGNodeStyleSetMargin(node, edge, value);
    }

    void setPadding(YGEdge edge, float value)
    {
      YGNodeStyleSetPadding(node, edge, value);
    }

    void setAlignItems(YGAlign align)
    {
      YGNodeStyleSetAlignItems(node, align);
    }

    void setJustifyContent(YGJustify justify)
    {
      YGNodeStyleSetJustifyContent(node, justify);
    }

    void layout(float width, float height)
    {
      calculateLayout(width, height);
    }

    void printLayout()
    {
      printNode(node, 0);
    }

  private:
    YGNodeRef node;

    YGNodeRef getYogaNode() const { return node; }

    void printNode(YGNodeRef node, int level)
    {
      float left = YGNodeLayoutGetLeft(node);
      float top = YGNodeLayoutGetTop(node);
      float width = YGNodeLayoutGetWidth(node);
      float height = YGNodeLayoutGetHeight(node);

      for (int i = 0; i < level; ++i)
        nsLog::Info("Node: Left={0}, Top={1}, Width={2}, Height={3}", left, top, width, height);

      for (uint32_t i = 0; i < YGNodeGetChildCount(node); ++i)
      {
        printNode(YGNodeGetChild(node, i), level + 1);
      }
    }
  };
} // namespace aperture::layout