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
  /**
   * @class InlineContainer
   * @brief Manages a collection of inline items, handling layout calculations.
   *
   * The InlineContainer class provides functionality to add items with specific dimensions and margins,
   * set container dimensions and padding.
   */
  class NS_APERTURE_DLL InlineContainer : public LayoutNode
  {
  public:
    InlineContainer(const Style& style)
      : LayoutNode(style)
    {
    }

    InlineContainer() {}

    void addChild(std::shared_ptr<LayoutNode> child) override
    {
      children_.push_back(child);
    }

    void calculateLayout(float width, float height) override
    {
      size_.width = width;
      size_.height = height;
      calculateInlineLayout(width);
    }

  private:
    void calculateInlineLayout(float containerWidth)
    {
      float xOffset = 0.0f;
      float yOffset = 0.0f;
      float lineHeight = 0.0f;

      for (auto& child : children_)
      {
        if (xOffset + child->size_.width > containerWidth)
        {
          xOffset = 0.0f; // Move to the next line
          yOffset += lineHeight;
          lineHeight = 0.0f;
        }

        child->position_ = {xOffset, yOffset};
        xOffset += child->size_.width;
        lineHeight = std::max(lineHeight, child->size_.height);
      }

      size_.height = yOffset + lineHeight; // Adjust container height
    }

  public:
    void layout(float width, float height)
    {
      calculateLayout(width, height);
    }

    void printLayout()
    {
      for (const auto& child : children_)
      {
        nsLog::Info("Child: Left={0}, Top={1}, Width={3}, Height={4}",
          child->position_.x,
          child->position_.y,
          child->size_.width,
          child->size_.height);
      }
    }
  };
} // namespace aperture::layout