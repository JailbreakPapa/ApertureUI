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

namespace aperture::layout
{
  class LayoutNode;
  class Style;

  enum class Direction
  {
    Row,
    Column
  };
  enum class JustifyContent
  {
    FlexStart,
    Start,
    Center,
    End,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly
  };
  enum class AlignItems
  {
    Start,
    Center,
    End,
    Stretch
  };

  struct Size
  {
    float width = 0;
    float height = 0;
  };

  struct Position
  {
    float x = 0;
    float y = 0;
  };

  class Style
  {
  public:
    Direction direction = Direction::Row;
    JustifyContent justifyContent = JustifyContent::Start;
    AlignItems alignItems = AlignItems::Start;
    float gap = 0;
    Size padding{};
    Size margin{};
    float flexGrow = 0;
    float flexShrink = 1;
    float flexBasis = 0;

    // Grid specific properties
    int gridColumnStart = 0;
    int gridColumnEnd = 0;
    int gridRowStart = 0;
    int gridRowEnd = 0;
  };
  enum class GridSizeType
  {
    Fixed,      // Absolute size (e.g., in px)
    Fractional, // Fractional size (e.g., 1fr, 2fr)
    Auto        // Automatically determined size
  };

  struct GridSize
  {
    GridSizeType type; // Type of the size
    float value;       // Value for the size (ignored for Auto)
  };
} // namespace aperture::layout
