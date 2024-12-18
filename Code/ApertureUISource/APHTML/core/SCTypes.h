#pragma once

namespace aperture
{
  enum class ScrollBehavior
  {
    Auto,    // Scroll using the context's configured setting.
    Smooth,  // Scroll using a smooth animation.
    Instant, // Scroll instantly.
  };

  enum class ScrollAlignment
  {
    Start,   // Align to the top or left edge of the parent element.
    Center,  // Align to the center of the parent element.
    End,     // Align to the bottom or right edge of the parent element.
    Nearest, // Align with minimal scroll change.
  };

  /**
      Defines behavior of Element::ScrollIntoView.
   */
  struct ScrollIntoViewOptions
  {
    ScrollIntoViewOptions(ScrollAlignment vertical = ScrollAlignment::Start, ScrollAlignment horizontal = ScrollAlignment::Nearest,
      ScrollBehavior behavior = ScrollBehavior::Instant)
      : vertical(vertical)
      , horizontal(horizontal)
      , behavior(behavior)
    {
    }
    ScrollAlignment vertical;
    ScrollAlignment horizontal;
    ScrollBehavior behavior;
  };
} // namespace aperture