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

#include <yoga/Yoga.h>
#include <APHTML/layout/Core/LayoutNode.h>

namespace aperture::layout
{
  class NS_APERTURE_DLL YogaLayoutNode : public LayoutNode
  {
    public:
    YogaLayoutNode()
      : yogaNode(YGNodeNew())
      , config({})
    {
    }

    virtual ~YogaLayoutNode()
    {
      for (auto child : children)
      {
        delete child;
      }
      YGNodeFree(yogaNode);
    }

    // Set layout properties
    void setConfig(const LayoutConfig& newConfig)
    {
      config = newConfig;
      applyConfig();
    }

    const LayoutConfig& getConfig() const { return config; }

    // Add child nodes
    void appendChild(YogaLayoutNode* child)
    {
      children.push_back(child);
      YGNodeInsertChild(yogaNode, child->getYogaNode(), children.size() - 1);
    }

    // Layout calculation
    void calculateLayout(float width = YGUndefined, float height = YGUndefined)
    {
      YGNodeCalculateLayout(yogaNode, width, height, YGDirectionLTR);
      for (auto child : children)
      {   
        child->calculateLayout(getComputedWidth(),getComputedHeight());
      }
    }

    // Get computed layout values
    float getComputedWidth() const { return YGNodeLayoutGetWidth(yogaNode); }
    float getComputedHeight() const { return YGNodeLayoutGetHeight(yogaNode); }
    float getComputedTop() const { return YGNodeLayoutGetTop(yogaNode); }
    float getComputedLeft() const { return YGNodeLayoutGetLeft(yogaNode); }
    public:
    YGNodeRef getYogaNode() { return yogaNode; }

  protected:
    void applyConfig()
    {
      // Map enum values to Yoga properties
      YGNodeStyleSetDisplay(yogaNode, config.display == Display::None ? YGDisplayNone : YGDisplayFlex);

      if (config.display == Display::Flex)
      {
        YGNodeStyleSetFlexDirection(
          yogaNode, static_cast<YGFlexDirection>(config.flexDirection));
      }

      YGNodeStyleSetAlignItems(yogaNode, static_cast<YGAlign>(config.alignItems));
      YGNodeStyleSetJustifyContent(
        yogaNode, static_cast<YGJustify>(config.justifyContent));

      YGNodeStyleSetPositionType(
        yogaNode, static_cast<YGPositionType>(config.positionType));

      YGNodeStyleSetFlexGrow(yogaNode, config.flexGrow);
      YGNodeStyleSetFlexShrink(yogaNode, config.flexShrink);

      YGNodeStyleSetWidth(yogaNode, config.width);
      YGNodeStyleSetHeight(yogaNode, config.height);

      // Margins
      YGNodeStyleSetMargin(yogaNode, YGEdgeTop, config.marginTop);
      YGNodeStyleSetMargin(yogaNode, YGEdgeBottom, config.marginBottom);
      YGNodeStyleSetMargin(yogaNode, YGEdgeLeft, config.marginLeft);
      YGNodeStyleSetMargin(yogaNode, YGEdgeRight, config.marginRight);
    }

  private:
    YGNodeRef yogaNode;
    LayoutConfig config;
    std::vector<LayoutNode*> children;
  };
} // namespace aperture::layout
