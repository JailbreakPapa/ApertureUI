#include <APHTML/Layout/Core/InlineBox.h>

using namespace aperture::layout;

InlineBox::~InlineBox() {
    YGNodeFree(yogaNode);
}

void InlineBox::setSize(float width, float height) {
    this->width = width;
    this->height = height;
    YGNodeStyleSetWidth(yogaNode, width);
    YGNodeStyleSetHeight(yogaNode, height);
}

void InlineBox::setBaseline(float baseline) {
    this->baseline = baseline;
}

void InlineBox::setVerticalAlign(VerticalAlign align, float length) {
    verticalAlign = align;
    if (align == VerticalAlign::Length) {
        verticalOffset = length;
    }
}

VerticalAlign InlineBox::getVerticalAlign() const {
    return verticalAlign;
}

float InlineBox::getVerticalOffset() const {
    return verticalOffset;
}

void InlineBox::addChild(std::shared_ptr<InlineBox> child) {
    children.push_back(child);
    YGNodeInsertChild(yogaNode, child->getYogaNode(), YGNodeGetChildCount(yogaNode));
}

YGNodeRef InlineBox::getYogaNode() {
    return yogaNode;
}

void InlineBox::layout(float availableWidth) {
    handleOverflow(availableWidth);

    for (auto& child : children) {
        child->layout(availableWidth);
    }

    YGNodeCalculateLayout(yogaNode, availableWidth, YGUndefined, YGDirectionLTR);
}

void InlineBox::handleOverflow(float availableWidth) {
    if (width > availableWidth) {
        // Logic to split child elements or adjust layout
        float remainingWidth = availableWidth;
        for (auto& child : children) {
            if (child->width > remainingWidth) {
                // Move child to a new line or handle splitting
                nsLog::Warning("Overflow detected for child with width: {0}. Moving Child to a new Row. ", child->width);
                remainingWidth = availableWidth;
            } else {
                remainingWidth -= child->width;
            }
        }
    }
}

void InlineBox::configureYogaNode() {
    YGNodeStyleSetDisplay(yogaNode, YGDisplayFlex);
    YGNodeStyleSetAlignItems(yogaNode, YGAlignBaseline);
    YGNodeSetContext(yogaNode, this);
    YGNodeSetBaselineFunc(yogaNode, calculateBaseline);
}

float InlineBox::calculateBaseline(YGNodeConstRef node, float width, float height) {
    InlineBox* box = static_cast<InlineBox*>(YGNodeGetContext(node));
    if (!box) return height;

    switch (box->getVerticalAlign()) {
        case VerticalAlign::Baseline:
            return box->baseline;
        case VerticalAlign::Sub:
            return box->baseline + 4;  // Example: Shift down by 4px
        case VerticalAlign::Super:
            return box->baseline - 4;  // Example: Shift up by 4px
        case VerticalAlign::TextTop:
            return 0;
        case VerticalAlign::TextBottom:
            return height;
        case VerticalAlign::Middle:
            return height / 2;
        case VerticalAlign::Length:
            return box->baseline - box->getVerticalOffset();
        default:
            return box->baseline;
    }
}

void InlineBox::render() {
    // Stub/TODO: Implement verification here.
}
