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
#include <APHTML/layout/Core/LayoutDefinitions.h>
#include <vector>
#include <yoga/YGValue.h>

namespace aperture::layout
{
  enum class Display
  {
    None,
    Flex,
    Grid
  };

  enum class FlexDirection
  {
    Row,
    Column,
    RowReverse,
    ColumnReverse
  };

  enum class Align
  {
    Auto,
    FlexStart,
    Center,
    FlexEnd,
    Stretch
  };

  enum class PositionType
  {
    Relative,
    Absolute
  };

  enum class LayoutBoxFlag : nsUInt16
  {
    // left to right
    LAYOUT_BOX_ROW = 0x002,
    // top to bottom
    LAYOUT_BOX_COLUMN = 0x003,

    // model (bit 1)
    // free layout
    LAYOUT_BOX_LAYOUT = 0x000,
    // flex model
    LAYOUT_BOX_FLEX = 0x002,

    // flex-wrap (bit 2)

    // single-line
    LAYOUT_BOX_NOWRAP = 0x000,
    // multi-line, wrap left to right
    LAYOUT_BOX_WRAP = 0x004,


    // justify-content (start, end, center, space-between)
    // at start of row/column
    LAYOUT_BOX_START = 0x008,
    // at center of row/column
    LAYOUT_BOX_MIDDLE = 0x000,
    // at end of row/column
    LAYOUT_BOX_END = 0x010,
    // insert spacing to stretch across whole row/column
    LAYOUT_BOX_JUSTIFY = 0x018

    // align-items
    // can be implemented by putting a flex container in a layout container,
    // then using LAY_TOP, LAY_BOTTOM, LAY_VFILL, LAY_VCENTER, etc.
    // FILL is equivalent to stretch/grow

    // align-content (start, end, center, stretch)
    // can be implemented by putting a flex container in a layout container,
    // then using LAY_TOP, LAY_BOTTOM, LAY_VFILL, LAY_VCENTER, etc.
    // FILL is equivalent to stretch; space-between is not supported.
  };
  enum class LayoutBoxDirection : nsUInt16
  {
    // anchor to left item or left side of parent
    LAYOUT_BOX_LEFT = 0x020,
    // anchor to top item or top side of parent
    LAYOUT_BOX_TOP = 0x040,
    // anchor to right item or right side of parent
    LAYOUT_BOX_RIGHT = 0x080,
    // anchor to bottom item or bottom side of parent
    LAYOUT_BOX_BOTTOM = 0x100,
    // anchor to both left and right item or parent borders
    LAYOUT_BOX_HFILL = 0x0a0,
    // anchor to both top and bottom item or parent borders
    LAYOUT_BOX_VFILL = 0x140,
    // center horizontally, with left margin as offset
    LAYOUT_BOX_HCENTER = 0x000,
    // center vertically, with top margin as offset
    LAYOUT_BOX_VCENTER = 0x000,
    // center in both directions, with left/top margin as offset
    LAYOUT_BOX_CENTER = 0x000,
    // anchor to all four directions
    LAYOUT_BOX_FILL = 0x1e0,
    // When in a wrapping container, put this element on a new line.
    // Drawing routines can read this via item pointers as needed after
    // performing layout calculations.
    LAYOUT_BOX_BREAK = 0x200
  };
  enum class VerticalAlign
  {
    Baseline,
    Sub,
    Super,
    TextTop,
    TextBottom,
    Middle,
    Length
  };
  struct LayoutConfig
  {
    Display display = Display::Flex;
    FlexDirection flexDirection = FlexDirection::Row;
    Align alignItems = Align::Stretch;
    JustifyContent justifyContent = JustifyContent::Start;
    PositionType positionType = PositionType::Relative;
    float flexGrow = 0.0f;
    float flexShrink = 1.0f;
    float width = YGUndefined; // Undefined by default (Yoga-compatible)
    float height = YGUndefined;

    // Margins, paddings, etc.
    float marginTop = 0.0f;
    float marginBottom = 0.0f;
    float marginLeft = 0.0f;
    float marginRight = 0.0f;
  };
} // namespace aperture::layout
