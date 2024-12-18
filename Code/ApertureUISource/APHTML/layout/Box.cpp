#include <APHTML/layout/Box.h>
#include <Foundation/Basics.h>
using namespace aperture::layout;

Box::Box() {}
Box::Box(nsVec2 content) : content(content) {}

Box::~Box() {}

nsVec2 Box::GetPosition(BoxArea area) const
{
	NS_ASSERT_ALWAYS(area != BoxArea::Auto,"Box::GetPosition(BoxArea area): Auto area is not supported for GetPosition");
	nsVec2 area_position(-GetEdge(BoxArea::Margin, BoxEdge::Left), -GetEdge(BoxArea::Margin, BoxEdge::Top));
	for (int i = 0; i < (int)area; i++)
	{
		area_position.x += area_edges[i][(int)BoxEdge::Left];
		area_position.y += area_edges[i][(int)BoxEdge::Top];
	}

	return area_position;
}

nsVec2 Box::GetSize() const
{
	return content;
}

nsVec2 Box::GetSize(BoxArea area) const
{
	NS_ASSERT_ALWAYS(area != BoxArea::Auto,"Box::GetSize(BoxArea area): Auto area is not supported for GetSize");
	nsVec2 area_size(content);
	for (int i = (int)area; i <= (int)BoxArea::Padding; i++)
	{
		area_size.x += (area_edges[i][(int)BoxEdge::Left] + area_edges[i][(int)BoxEdge::Right]);
		area_size.y += (area_edges[i][(int)BoxEdge::Top] + area_edges[i][(int)BoxEdge::Bottom]);
	}

	return area_size;
}

void Box::SetContent(nsVec2 _content)
{
	content = _content;
}

void Box::SetEdge(BoxArea area, BoxEdge edge, float size)
{
	NS_ASSERT_ALWAYS(area != BoxArea::Auto,"Box::SetEdge(BoxArea area, BoxEdge edge, float size): Auto area is not supported for SetEdge");
	area_edges[(int)area][(int)edge] = size;
}

float Box::GetEdge(BoxArea area, BoxEdge edge) const
{
	NS_ASSERT_ALWAYS(area != BoxArea::Auto,"Box::GetEdge(BoxArea area, BoxEdge edge): Auto area is not supported for GetEdge");
	return area_edges[(int)area][(int)edge];
}

float Box::GetCumulativeEdge(BoxArea area, BoxEdge edge) const
{
	NS_ASSERT_ALWAYS(area != BoxArea::Auto,"Box::GetCumulativeEdge(BoxArea area, BoxEdge edge): Auto area is not supported for GetCumulativeEdge");
	float size = 0;
	int max_area = nsMath::Min((int)area, (int)BoxArea::Padding);
	for (int i = 0; i <= max_area; i++)
		size += area_edges[i][(int)edge];

	return size;
}

float Box::GetSizeAcross(BoxDirection direction, BoxArea area_outer, BoxArea area_inner) const
{
	static_assert((int)BoxDirection::Horizontal == 1 && (int)BoxDirection::Vertical == 0, "");
	NS_ASSERT_ALWAYS((int)area_outer <= (int)area_inner && (int)direction <= 1 && area_inner != BoxArea::Auto,"Box::GetSizeAcross(BoxDirection direction, BoxArea area_outer, BoxArea area_inner): Auto area is not supported for GetSizeAcross");

	float size = 0.0f;

	if (area_inner == BoxArea::Content)
		size = (direction == BoxDirection::Horizontal ? content.x : content.y);

	for (int i = (int)area_outer; i <= (int)area_inner && i < (int)BoxArea::Content; i++)
		size += (area_edges[i][(int)BoxEdge::Top + (int)direction] + area_edges[i][(int)BoxEdge::Bottom + (int)direction]);

	return size;
}

nsVec2 Box::GetFrameSize(BoxArea area) const
{
	if (area == BoxArea::Content)
		return content;

	return {
		area_edges[(int)area][(int)BoxEdge::Right] + area_edges[(int)area][(int)BoxEdge::Left],
		area_edges[(int)area][(int)BoxEdge::Top] + area_edges[(int)area][(int)BoxEdge::Bottom],
	};
}

bool Box::operator==(const Box& rhs) const
{
	return content == rhs.content && memcmp(area_edges, rhs.area_edges, sizeof(area_edges)) == 0;
}

bool Box::operator!=(const Box& rhs) const
{
	return !(*this == rhs);
}