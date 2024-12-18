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

namespace aperture::layout {

enum class BoxEdge { Top, Right, Bottom, Left };
enum class BoxDirection { Vertical, Horizontal };
enum class BoxArea { Margin, Border, Padding, Content, Auto };
/**
    Stores a box with four sized areas; content, padding, a border and margin. See
    http://www.w3.org/TR/REC-CSS2/box.html#box-dimensions for a diagram.
 */

class NS_APERTURE_DLL Box {
public:
	static constexpr int num_areas = 3; // ignores content box
	static constexpr int num_edges = 4;

	/// Initialises a zero-sized box.
	Box();
	/// Initialises a box with a default content area and no padding, borders and margins.
	explicit Box(nsVec2 content);
	~Box();

	/// Returns the top-left position of one of the box's areas, relative to the top-left of the border area. This
	/// means the position of the margin area is likely to be negative.
	/// @param area[in] The desired area.
	/// @return The position of the area.
	nsVec2 GetPosition(BoxArea area = BoxArea::Content) const;
	/// Returns the size of the box's content area.
	/// @return The size of the content area.
	nsVec2 GetSize() const;
	/// Returns the size of one of the box's areas. This will include all inner areas.
	/// @param area[in] The desired area.
	/// @return The size of the requested area.
	nsVec2 GetSize(BoxArea area) const;

	/// Sets the size of the content area.
	/// @param content[in] The size of the new content area.
	void SetContent(nsVec2 content);
	/// Sets the size of one of the edges of one of the box's outer areas.
	/// @param area[in] The area to change.
	/// @param edge[in] The area edge to change.
	/// @param size[in] The new size of the area segment.
	void SetEdge(BoxArea area, BoxEdge edge, float size);

	/// Returns the size of one of the area edges.
	/// @param area[in] The desired area.
	/// @param edge[in] The desired edge.
	/// @return The size of the requested area edge.
	float GetEdge(BoxArea area, BoxEdge edge) const;
	/// Returns the cumulative size of one edge up to one of the box's areas.
	/// @param area[in] The area to measure up to (and including). So, Margin will return the width of the margin, and Padding will be the sum of the
	/// margin, border and padding.
	/// @param edge[in] The desired edge.
	/// @return The cumulative size of the edge.
	float GetCumulativeEdge(BoxArea area, BoxEdge edge) const;

	/// Returns the size along a single direction of the given 'area', including all inner areas up-to and including 'area_end'.
	/// @param direction The desired direction.
	/// @param area The widest area to include.
	/// @param area_end The last area to include, anything inside this is excluded.
	/// @example GetSizeAcross(Horizontal, Border, Padding) returns the total width of the horizontal borders and paddings.
	float GetSizeAcross(BoxDirection direction, BoxArea area, BoxArea area_end = BoxArea::Content) const;

	/// Returns the size of the frame defined by the given area, not including inner areas.
	/// @param area The area to use.
	nsVec2 GetFrameSize(BoxArea area) const;

	/// Compares the size of the content area and the other area edges.
	/// @return True if the boxes represent the same area.
	bool operator==(const Box& rhs) const;
	/// Compares the size of the content area and the other area edges.
	/// @return True if the boxes do not represent the same area.
	bool operator!=(const Box& rhs) const;

private:
	nsVec2 content;
	float area_edges[num_areas][num_edges] = {};
};

} // namespace aperture::layout