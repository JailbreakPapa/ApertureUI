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

#include <APHTML/layout/Core/YogaLayoutNode.h>
#include <map>
#include <yoga/Yoga.h>

namespace aperture::layout
{
  class GridNode : public YogaLayoutNode
  {
  public:
    // Set row sizes using GridSize structures
    void setRowSizes(const std::vector<GridSize>& sizes)
    {
      rowSizes = sizes;
    }

    // Set column sizes using GridSize structures
    void setColumnSizes(const std::vector<GridSize>& sizes)
    {
      columnSizes = sizes;
    }

    // Define the area for a child node (rowStart, rowEnd, colStart, colEnd)
    void setGridArea(LayoutNode* child, int rowStart, int rowEnd, int colStart, int colEnd)
    {
      childAreas[static_cast<YogaLayoutNode*>(child)] = std::make_tuple(rowStart, rowEnd, colStart, colEnd);
    }

    void calculateLayout(float width = YGUndefined, float height = YGUndefined) override
    {
      // Calculate grid cell sizes
      calculateGridSizes(width, height);

      // Place child nodes into the computed grid
      placeGridItems();

      // Let Yoga handle the final layout
      LayoutNode::calculateLayout(width, height);
    }

  private:
    std::vector<GridSize> rowSizes;                                   // Sizes of rows
    std::vector<GridSize> columnSizes;                                // Sizes of columns
    std::map<YogaLayoutNode*, std::tuple<int, int, int, int>> childAreas; // Map of child areas

    void calculateGridSizes(float totalWidth, float totalHeight)
    {
      float totalRowFraction = 0.0f, totalColumnFraction = 0.0f;
      float remainingHeight = totalHeight, remainingWidth = totalWidth;

      // Calculate total fractions and subtract fixed sizes for rows
      for (const auto& size : rowSizes)
      {
        if (size.type == GridSizeType::Fixed)
        {
          remainingHeight -= size.value;
        }
        else if (size.type == GridSizeType::Fractional)
        {
          totalRowFraction += size.value;
        }
      }

      // Calculate total fractions and subtract fixed sizes for columns
      for (const auto& size : columnSizes)
      {
        if (size.type == GridSizeType::Fixed)
        {
          remainingWidth -= size.value;
        }
        else if (size.type == GridSizeType::Fractional)
        {
          totalColumnFraction += size.value;
        }
      }

      // Allocate sizes for rows
      for (auto& size : rowSizes)
      {
        if (size.type == GridSizeType::Fractional)
        {
          size.value = (remainingHeight * size.value) / totalRowFraction;
        }
        else if (size.type == GridSizeType::Auto)
        {
          size.value = remainingHeight / rowSizes.size(); // Simple auto distribution
        }
      }

      // Allocate sizes for columns
      for (auto& size : columnSizes)
      {
        if (size.type == GridSizeType::Fractional)
        {
          size.value = (remainingWidth * size.value) / totalColumnFraction;
        }
        else if (size.type == GridSizeType::Auto)
        {
          size.value = remainingWidth / columnSizes.size(); // Simple auto distribution
        }
      }
    }

    void placeGridItems()
    {
      for (auto& [child, area] : childAreas)
      {
        int rowStart, rowEnd, colStart, colEnd;
        std::tie(rowStart, rowEnd, colStart, colEnd) = area;

        float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;

        // Calculate x and width
        for (int i = 0; i < colStart; ++i)
        {
          x += columnSizes[i].value;
        }
        for (int i = colStart; i < colEnd; ++i)
        {
          width += columnSizes[i].value;
        }

        // Calculate y and height
        for (int i = 0; i < rowStart; ++i)
        {
          y += rowSizes[i].value;
        }
        for (int i = rowStart; i < rowEnd; ++i)
        {
          height += rowSizes[i].value;
        }

        YGNodeStyleSetPosition(child->getYogaNode(), YGEdgeLeft, x);
        YGNodeStyleSetPosition(child->getYogaNode(), YGEdgeTop, y);
        YGNodeStyleSetWidth(child->getYogaNode(), width);
        YGNodeStyleSetHeight(child->getYogaNode(), height);
      }
    }
  };
} // namespace aperture::layout
