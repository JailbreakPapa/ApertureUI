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

namespace aperture::css
{
  /// @brief Enum class for Currently Supported CSS3 Syntax Properties
  enum class CSSSyntaxProperties
  {
    // NOTE: Fundamental CSS3 Syntax, Don't Reference As Properties!
    fundamental__identifier,     // identifier
    fundamental__string_literal, // actual string literal (e.g., 'value' or "value")
    fundamental__value,          // value
    fundamental__hexvalue,       // hex value
    fundamental__property,       // property
    fundamental__block,          // block
    fundamental__css_rule,       // CSS Rule

    // Aperture UI Specific CSS3 Syntax Properties
    aperture_sdf_font_support, // apui-sdf-font: true, false, auto

    // W3C Compliant CSS3 Syntax Properties
    align_content,
    align_items,
    align_self,
    all,
    animation,
    animation_delay,
    animation_direction,
    animation_duration,
    animation_fill_mode,
    animation_iteration_count,
    animation_name,
    animation_play_state,
    animation_timing_function,
    backface_visibility,
    background,
    background_attachment,
    background_blend_mode,
    background_clip,
    background_color,
    background_image,
    background_origin,
    background_position,
    background_repeat,
    background_size,
    border,
    border_bottom,
    border_bottom_color,
    border_bottom_left_radius,
    border_bottom_right_radius,
    border_bottom_style,
    border_bottom_width,
    border_collapse,
    border_color,
    border_image,
    border_image_outset,
    border_image_repeat,
    border_image_slice,
    border_image_source,
    border_image_width,
    border_left,
    border_left_color,
    border_left_style,
    border_left_width,
    border_radius,
    border_right,
    border_right_color,
    border_right_style,
    border_right_width,
    border_spacing,
    border_style,
    border_top,
    border_top_color,
    border_top_left_radius,
    border_top_right_radius,
    border_top_style,
    border_top_width,
    border_width,
    bottom,
    box_decoration_break,
    box_shadow,
    box_sizing,
    caption_side,
    caret_color,
    charset, //@charset
    clear,
    clip,
    color,
    column_count,
    column_fill,
    column_gap,
    column_rule,
    column_rule_color,
    column_rule_style,
    column_rule_width,
    column_span,
    column_width,
    columns,
    content,
    counter_increment,
    counter_reset,
    cursor,
    direction,
    display,
    empty_cells,
    filter,
    flex,
    flex_basis,
    flex_direction,
    flex_flow,
    flex_grow,
    flex_shrink,
    flex_wrap,
    float_, // float
    font,
    font_face, //@font_face
    font_family,
    font_kerning,
    font_size,
    font_size_adjust,
    font_stretch,
    font_style,
    font_variant,
    font_weight,
    grid,
    grid_area,
    grid_auto_columns,
    grid_auto_flow,
    grid_auto_rows,
    grid_column,
    grid_column_end,
    grid_column_gap,
    grid_column_start,
    grid_gap,
    grid_row,
    grid_row_end,
    grid_row_gap,
    grid_row_start,
    grid_template,
    grid_template_areas,
    grid_template_columns,
    grid_template_rows,
    hanging_punctuation,
    height,
    hyphens,
    import, //@import
    isolation,
    justify_content,
    keyframes, // @keyframes
    left,
    letter_spacing,
    line_height,
    list_style,
    list_style_image,
    list_style_position,
    list_style_type,
    margin,
    margin_bottom,
    margin_left,
    margin_right,
    margin_top,
    max_height,
    max_width,
    media, //@media
    min_height,
    min_width,
    mix_blend_mode,
    object_fit,
    object_position,
    opacity,
    order,
    outline,
    outline_color,
    outline_offset,
    outline_style,
    outline_width,
    overflow,
    overflow_x,
    overflow_y,
    padding,
    padding_bottom,
    padding_left,
    padding_right,
    padding_top,
    page_break_after,
    page_break_before,
    page_break_inside,
    perspective,
    perspective_origin,
    pointer_events,
    position,
    quotes,
    resize,
    right,
    scroll_behavior,
    tab_size,
    table_layout,
    text_align,
    text_align_last,
    text_decoration,
    text_decoration_color,
    text_decoration_line,
    text_decoration_style,
    text_indent,
    text_justify,
    text_overflow,
    text_shadow,
    text_transform,
    top,
    transform,
    transform_origin,
    transform_style,
    transition,
    transition_delay,
    transition_duration,
    transition_property,
    transition_timing_function,
    unicode_bidi,
    user_select,
    vertical_align,
    visibility,
    white_space,
    width,
    word_break,
    word_spacing,
    word_wrap,
    z_index,
    language,
    direction
  };
} // namespace aperture::css
