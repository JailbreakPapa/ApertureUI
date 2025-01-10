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
  class NS_APERTURE_DLL InlineContainer
  {
  public:
    /**
     * @brief Constructs a new InlineContainer object.
     */
    InlineContainer();

    /**
     * @brief Destructs the InlineContainer object.
     */
    ~InlineContainer();

    /**
     * @brief Adds an item to the container with specified width, height, and optional margin.
     *
     * @param width The width of the item.
     * @param height The height of the item.
     * @param margin The margin around the item. Defaults to 0.0f.
     */
    void addItem(float width, float height, float margin = 0.0f);

    /**
     * @brief Sets the width of the container.
     *
     * @param width The desired width of the container.
     */
    void setContainerWidth(float width);

    /**
     * @brief Sets the height of the container.
     *
     * @param height The desired height of the container.
     */
    void setContainerHeight(float height);

    /**
     * @brief Sets the padding of the container.
     *
     * @param padding The padding value to apply to the container.
     */
    void setPadding(float padding);

    /**
     * @brief Calculates the layout of the container and its contained items.
     *
     * This function computes the layout based on the current container dimensions, padding, and
     * the dimensions and margins of the contained items using the Yoga layout engine.
     */
    void calculateLayout();

    /**
     * @brief Prints the current layout configuration of the container and its items.
     *
     * This function outputs the layout details, such as positions and sizes, of the container and
     * each of its contained items for debugging or informational purposes.
     */
    void printLayout() const;

  private:
    YGNodeRef containerNode;
    std::vector<std::unique_ptr<YGNodeRef>> itemNodes;
  };
} // namespace aperture::layout