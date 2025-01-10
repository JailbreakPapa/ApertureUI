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
#include <APHTML/core/BaseProperty.h>
#include <APHTML/core/ID.h>
#include <APHTML/dom/DOMElement.h>

namespace aperture
{
  namespace css
  {
    class StyleSheet;
  }
  namespace layout
  {
    class Box;
    enum class BoxArea;
  } // namespace layout
  class NS_APERTURE_DLL Element : public dom::DOMElement
  {
  public:
    Element();
    ~Element();

    /// Returns the active style sheet for this element. This may be nullptr.
    /// @return The element's style sheet.
    virtual const css::StyleSheet* GetStyleSheet() const = 0;

    /// Fills a string with the full address of this element.
    /// @param[in] include_pseudo_classes True to include the pseudo-classes of the leaf element.
    /// @param[in] include_parents True to include the address of all parent elements.
    /// @return The address of the element.
    nsString GetAddress(bool include_pseudo_classes = false, bool include_parents = true) const;

    /// Sets the position of this element, as a two-dimensional offset from another element.
    /// @param[in] offset The offset (in pixels) of our primary box's top-left border corner from our offset parent's top-left border corner.
    /// @param[in] offset_parent The element this element is being positioned relative to.
    /// @param[in] offset_fixed True if the element is fixed in place (and will not scroll), false if not.
    void SetOffset(nsVec2 offset, Element* offset_parent, bool offset_fixed = false);
    /// Returns the position of the top-left corner of one of the areas of this element's primary box, relative to its
    /// offset parent's top-left border corner.
    /// @param[in] area The desired area position.
    /// @return The relative offset.
    nsVec2 GetRelativeOffset(layout::BoxArea area);
    /// Returns the position of the top-left corner of one of the areas of this element's primary box, relative to
    /// the element root.
    /// @param[in] area The desired area position.
    /// @return The absolute offset.
    nsVec2 GetAbsoluteOffset(layout::BoxArea area);

    /// Sets an alternate area to use as the clip area.
    /// @param[in] clip_area The box area to use as the element's clip area.
    void SetClipArea(layout::BoxArea clip_area);
    /// Returns the area the element uses as its clip area.
    /// @return The box area used as the element's clip area.
    layout::BoxArea GetClipArea() const;

    /// Sets the dimensions of the element's scrollable overflow rectangle. This is the tightest fitting box surrounding
    /// all of this element's logical children, and the element's padding box.
    /// @param[in] scrollable_overflow_rectangle The dimensions of the box's scrollable content.
    /// @param[in] clamp_scroll_offset Clamp scroll offset to the new rectangle, should only be set when the final overflow size has been determined.
    void SetScrollableOverflowRectangle(nsVec2 scrollable_overflow_rectangle, bool clamp_scroll_offset);
    /// Sets the box describing the size of the element, and removes all others.
    /// @param[in] box The new dimensions box for the element.
    void SetBox(const layout::Box& box);
    /// Adds a box to the end of the list describing this element's geometry.
    /// @param[in] box The auxiliary box for the element.
    /// @param[in] offset The offset of the box relative to the top left border corner of the element.
    void AddBox(const layout::Box& box, nsVec2 offset);
    /// Returns the main box describing the size of the element.
    /// @return The box.
    const layout::Box& GetBox();
    /// Returns one of the boxes describing the size of the element.
    /// @param[in] index The index of the desired box, with 0 being the main box. If outside of bounds, the main box will be returned.
    /// @param[out] offset The offset of the box relative to the element's border box.
    /// @return The requested box.
    const layout::Box& GetBox(int index, nsVec2& offset);
    /// Returns the number of boxes making up this element's geometry.
    /// @return the number of boxes making up this element's geometry.
    int GetNumBoxes();

    /// Returns the baseline of the element, in pixel offset from the element's bottom margin edge (positive up).
    /// @return The element's baseline. The default element will return 0.
    virtual float GetBaseline() const;
    /// Gets the intrinsic dimensions of this element, if it is a replaced element with an inherent size. This size will
    /// only be overriden by a styled width or height.
    /// @param[out] dimensions The dimensions to size, if appropriate.
    /// @param[out] ratio The intrinsic ratio (width/height), if appropriate.
    /// @return True if the element is a replaced element with intrinsic dimensions, false otherwise.
    virtual bool GetIntrinsicDimensions(nsVec2& dimensions, float& ratio);
    /// Returns true if the element is replaced, thereby handling its own rendering.
    /// @return True if the element is a replaced element.
    bool IsReplaced();

    /// Checks if a given point in screen coordinates lies within the bordered area of this element.
    /// @param[in] point The point to test.
    /// @return True if the element is within this element, false otherwise.
    virtual bool IsPointWithinElement(nsVec2 point);

    /// Returns the visibility of the element.
    /// @param[in] include_ancestors Check parent elements for visibility
    /// @return True if the element is visible, false otherwise.
    bool IsVisible(bool include_ancestors = false) const;
    /// Returns the z-index of the element.
    /// @return The element's z-index.
    float GetZIndex() const;

    /** @name Properties
     */
    //@{
    /// Sets a local property override on the element.
    /// @param[in] name The name of the new property.
    /// @param[in] value The new property to set.
    /// @return True if the property parsed successfully, false otherwise.
    bool SetProperty(const nsString& name, const nsString& value);
    /// Sets a local property override on the element to a pre-parsed value.
    /// @param[in] id The id of the new property.
    /// @param[in] property The parsed property to set.
    /// @return True if the property was set successfully, false otherwise.
    template <typename T>
    bool SetProperty(PropertyId id, const Property<T>& property);
    /// Removes a local property override on the element; its value will revert to that defined in the style sheet.
    /// @param[in] name The name of the local property definition to remove.
    void RemoveProperty(const nsString& name);
    void RemoveProperty(PropertyId id);
    /// Returns one of this element's properties. If the property is not defined for this element and not inherited
    /// from an ancestor, the default value will be returned.
    /// @param[in] name The name of the property to fetch the value for.
    /// @return The value of this property for this element, or nullptr if no property exists with the given name.
    template <typename T>
    const Property<T>* GetProperty(const nsString& name);
    template <typename T>
    const Property<T>* GetProperty(PropertyId id);
    /// Returns the values of one of this element's properties.
    /// @param[in] name The name of the property to get.
    /// @return The value of this property.
    template <typename T>
    T GetProperty(const nsString& name);
    /// Returns one of this element's properties. If this element is not defined this property, nullptr will be
    /// returned.
    /// @param[in] name The name of the property to fetch the value for.
    /// @return The value of this property for this element, or nullptr if this property has not been explicitly defined for this element.
    template <typename T>
    const Property<T>* GetLocalProperty(const nsString& name);
    template <typename T>
    const Property<T>* GetLocalProperty(PropertyId id);

    /// Resolves a length to its canonical unit ('px').
    /// @param[in] value The numeric value.
    /// @return The resolved value in their canonical unit, or zero if it could not be resolved.
    /// @note Font-relative and context-relative units will be resolved against this element's computed values and its context.
    float ResolveLength(core::NumericValue value);
    /// Resolves a numeric value with units of number, percentage, length, or angle to their canonical unit (unit-less, 'px', or 'rad').
    /// Numbers and percentages are scaled by the base value and returned.
    /// @param[in] value The value to be resolved.
    /// @param[in] base_value The value that is scaled by the number or percentage value, if applicable.
    /// @return The resolved value in their canonical unit, or zero if it could not be resolved.
    float ResolveNumericValue(core::NumericValue value, float base_value);

    /// Returns the size of the containing block. Often percentages are scaled relative to this.
    nsVec2 GetContainingBlock();

    /// Project a 2D point in pixel coordinates onto the element's plane.
    /// @param[in-out] point The point to project in, and the resulting projected point out.
    /// @return True on success, false if transformation matrix is singular.
    bool Project(nsVec2& point) const noexcept;
  };
} // namespace aperture