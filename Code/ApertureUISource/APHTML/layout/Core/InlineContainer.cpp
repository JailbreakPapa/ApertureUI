#include <APHTML/Layout/Core/InlineContainer.h>

aperture::layout::InlineContainer::InlineContainer()
{
  containerNode = YGNodeNew();
  YGNodeStyleSetFlexDirection(containerNode, YGFlexDirectionRow);
  YGNodeStyleSetJustifyContent(containerNode, YGJustifyFlexStart);
  YGNodeStyleSetAlignItems(containerNode, YGAlignCenter);
}

aperture::layout::InlineContainer::~InlineContainer()
{
  YGNodeFree(containerNode);
  for (auto& itemNode : itemNodes)
  {
    YGNodeFree(*itemNode);
  }
}

void aperture::layout::InlineContainer::addItem(float width, float height, float margin)
{
  auto itemNode = std::make_unique<YGNodeRef>(YGNodeNew());
  YGNodeStyleSetWidth(*itemNode, width);
  YGNodeStyleSetHeight(*itemNode, height);
  YGNodeStyleSetMargin(*itemNode, YGEdgeAll, margin);

  YGNodeInsertChild(containerNode, *itemNode, YGNodeGetChildCount(containerNode));
  itemNodes.push_back(std::move(itemNode));
}

void aperture::layout::InlineContainer::setContainerWidth(float width)
{
  YGNodeStyleSetWidth(containerNode, width);
}

void aperture::layout::InlineContainer::setContainerHeight(float height)
{
  YGNodeStyleSetHeight(containerNode, height);
}

void aperture::layout::InlineContainer::setPadding(float padding)
{
  YGNodeStyleSetPadding(containerNode, YGEdgeAll, padding);
}

void aperture::layout::InlineContainer::calculateLayout()
{
  YGNodeCalculateLayout(containerNode, YGUndefined, YGUndefined, YGDirectionLTR);
}

void aperture::layout::InlineContainer::printLayout() const
{
  nsLog::Debug("Container Layout: \n");
  auto containerLayout = YGNodeLayoutGetWidth(containerNode);
  nsLog::Debug("Width: {0}", containerLayout);

  for (size_t i = 0; i < itemNodes.size(); ++i)
  {
    auto* itemNode = itemNodes[i].get();
    auto x = YGNodeLayoutGetLeft(*itemNode);
    auto y = YGNodeLayoutGetTop(*itemNode);
    auto width = YGNodeLayoutGetWidth(*itemNode);
    auto height = YGNodeLayoutGetHeight(*itemNode);

    nsLog::Debug("Item {0} : x= {1}, y= {2}, width={3}, height={4}", i + 1, x, y,
      width, height);
  }
}
