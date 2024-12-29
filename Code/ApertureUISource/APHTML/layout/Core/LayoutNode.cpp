#include "LayoutNode.h"



void aperture::layout::LayoutNode::addChild(std::shared_ptr<LayoutNode> child)
{
  children_.push_back(child);
}

void aperture::layout::LayoutNode::calculateLayout(const Size& availableSpace)
{
  if (style_.direction == Direction::Row)
  {
    calculateRowLayout(availableSpace);
  }
  else
  {
    calculateColumnLayout(availableSpace);
  }

  positionChildren();
}

void aperture::layout::LayoutNode::calculateLayout(float width, float height)
{
  Size size;
  size.width = width;
  size.height = height;
  calculateLayout(size_);
}

void aperture::layout::LayoutNode::calculateRowLayout(const Size& availableSpace)
{
  float totalFlexGrow = 0;
  float totalSpace = 0;

  // First pass: calculate total flex grow and initial sizes
  for (const auto& child : children_)
  {
    totalFlexGrow += child->style_.flexGrow;
    totalSpace += child->style_.flexBasis;
  }

  float remainingSpace = availableSpace.width - totalSpace;

  // Second pass: distribute remaining space according to flex grow
  if (remainingSpace > 0 && totalFlexGrow > 0)
  {
    float spacePerFlex = remainingSpace / totalFlexGrow;

    for (const auto& child : children_)
    {
      float childWidth = child->style_.flexBasis +
                         (child->style_.flexGrow * spacePerFlex);
      child->size_.width = childWidth;
      child->size_.height = availableSpace.height -
                            (child->style_.margin.height * 2);
    }
  }
}

void aperture::layout::LayoutNode::calculateColumnLayout(const Size& availableSpace)
{
  // Similar to row layout but working with height instead of width
  float totalFlexGrow = 0;
  float totalSpace = 0;

  for (const auto& child : children_)
  {
    totalFlexGrow += child->style_.flexGrow;
    totalSpace += child->style_.flexBasis;
  }

  float remainingSpace = availableSpace.height - totalSpace;

  if (remainingSpace > 0 && totalFlexGrow > 0)
  {
    float spacePerFlex = remainingSpace / totalFlexGrow;

    for (const auto& child : children_)
    {
      float childHeight = child->style_.flexBasis +
                          (child->style_.flexGrow * spacePerFlex);
      child->size_.height = childHeight;
      child->size_.width = availableSpace.width -
                           (child->style_.margin.width * 2);
    }
  }
}

void aperture::layout::LayoutNode::positionChildren()
{
  float currentPosition = 0;

  for (const auto& child : children_)
  {
    if (style_.direction == Direction::Row)
    {
      child->position_.x = currentPosition + child->style_.margin.width;

      // Handle align items
      switch (style_.alignItems)
      {
        case AlignItems::Center:
          child->position_.y = (size_.height - child->size_.height) / 2;
          break;
        case AlignItems::End:
          child->position_.y = size_.height - child->size_.height -
                               child->style_.margin.height;
          break;
        default: // Start or Stretch
          child->position_.y = child->style_.margin.height;
      }

      currentPosition += child->size_.width +
                         (child->style_.margin.width * 2) +
                         style_.gap;
    }
    else
    {
      child->position_.y = currentPosition + child->style_.margin.height;

      // Handle justify content
      switch (style_.justifyContent)
      {
        case JustifyContent::Center:
          child->position_.x = (size_.width - child->size_.width) / 2;
          break;
        case JustifyContent::End:
          child->position_.x = size_.width - child->size_.width -
                               child->style_.margin.width;
          break;
        default: // Start
          child->position_.x = child->style_.margin.width;
      }

      currentPosition += child->size_.height +
                         (child->style_.margin.height * 2) +
                         style_.gap;
    }

    // Recursively layout children
    child->calculateLayout(child->size_);
  }
}
