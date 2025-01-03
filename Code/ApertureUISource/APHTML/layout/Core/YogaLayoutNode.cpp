#include <APHTML/Layout/Core/YogaLayoutNode.h>

aperture::layout::YogaLayoutNode::YogaLayoutNode()
  : yogaNode(YGNodeNew())
  , config({})
{
}

aperture::layout::YogaLayoutNode::~YogaLayoutNode()
{
  for (auto child : children)
  {
    delete child;
  }
  YGNodeFree(yogaNode);
}

void aperture::layout::YogaLayoutNode::setConfig(const LayoutConfig& newConfig)
{
  config = newConfig;
  applyConfig();
}

const aperture::layout::LayoutConfig& aperture::layout::YogaLayoutNode::getConfig() const
{
  return config;
}

void aperture::layout::YogaLayoutNode::appendChild(YogaLayoutNode* child)
{
  children.push_back(child);
  YGNodeInsertChild(yogaNode, child->getYogaNode(), children.size() - 1);
}

void aperture::layout::YogaLayoutNode::calculateLayout(float width /*= YGUndefined*/, float height /*= YGUndefined*/)
{
  YGNodeCalculateLayout(yogaNode, width, height, YGDirectionLTR);
  for (auto child : children)
  {
    child->calculateLayout(getComputedWidth(), getComputedHeight());
  }
}

float aperture::layout::YogaLayoutNode::getComputedWidth() const
{
  return YGNodeLayoutGetWidth(yogaNode);
}

float aperture::layout::YogaLayoutNode::getComputedHeight() const
{
  return YGNodeLayoutGetHeight(yogaNode);
}

float aperture::layout::YogaLayoutNode::getComputedTop() const
{
  return YGNodeLayoutGetTop(yogaNode);
}

float aperture::layout::YogaLayoutNode::getComputedLeft() const
{
  return YGNodeLayoutGetLeft(yogaNode);
}

YGNodeRef aperture::layout::YogaLayoutNode::getYogaNode()
{
  return yogaNode;
}

void aperture::layout::YogaLayoutNode::applyConfig()
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
