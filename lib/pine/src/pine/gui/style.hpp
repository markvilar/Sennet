#pragma once

#include "pine/utils/math.hpp"

namespace pine::gui {

void set_dark_theme();

enum Color : int {
    TEXT = 0,
    TEXT_DISABLED = 1,
    WINDOW_BG = 2,
    CHILD_BG = 3,
    POPUP_BG = 4,
    BORDER = 5,
    BORDER_SHADOW = 6,
    FRAME_BG = 7,
    FRAME_BG_HOVERED = 8,
    FRAME_BG_ACTIVE = 9,
    TITLE_BG = 10,
    TITLE_BG_ACTIVE = 11,
    TITLE_BG_COLLAPSED = 12,
    MENUBAR_BG = 13,
    SCROLLBAR_BG = 14,
    SCROLLBAR_GRAB = 15,
    SCROLLBAR_GRAB_HOVERED = 16,
    SCROLLBAR_GRAB_ACTIVE = 17,
    CHECKMARK = 18,
    SLIDER_GRAB = 19,
    SLIDER_GRAB_ACTIVE = 20,
    BUTTON = 21,
    BUTTON_HOVERED = 22,
    BUTTON_ACTIVE = 23,
    HEADER = 24,
    HEADER_HOVERED = 25,
    HEADER_ACTIVE = 26,
    SEPARATOR = 27,
    SEPARATOR_HOVERED = 28,
    SEPARATOR_ACTIVE = 29,
    RESIZE_GRIP = 30,
    RESIZE_GRIP_HOVERED = 31,
    RESIZE_GRIP_ACTIVE = 32,
    TAB = 33,
    TAB_HOVERED = 34,
    TAB_ACTIVE = 35,
    TAB_UNFOCUSED = 36,
    TAB_UNFOCUSED_ACTIVE = 37,
    DOCKING_PREVIEW = 38,
    DOCKING_EMPTY_BG = 39,
    PLOT_LINES = 40,
    PLOT_LINES_HOVERED = 41,
    PLOT_HISTOGRAM = 42,
    PLOT_HISTOGRAM_HOVERED = 43,
    TABLE_HEADER_BG = 44,
    TABLE_BORDER_STRONG = 45,
    TABLE_BORDER_LIGHT = 46,
    TABLE_ROW_BG = 47,
    TABLE_ROW_BG_ALT = 48,
    TEXT_SELECTED_BG = 49,
    DRAG_DROP_TARGET = 50,
    NAV_HIGHLIGHT = 51,
    NAV_WINDOWING_HIGHLIGHT = 52,
    NAV_WINDOWING_DIM_BG = 53,
    MODAL_WINDOW_DIM_BG = 54,
    COUNT = 55,
};

struct CustomStyle {
    float alpha = 1.0f;
    float disabled_alpha = 0.6f;
    Vec2 window_padding = Vec2(8.0f, 8.0f);
    float window_rounding = 0.0f;
    float window_border_size = 1.0f;
    Vec2 window_min_size = Vec2(32.0f, 32.0f);
    Vec2 window_title_align = Vec2(0.0f, 0.5f);
    float child_rounding = 0.0f;
    float child_border_size = 1.0f;
    float popup_rounding = 0.0f;
    float popup_border_size = 1.0f;
    Vec2 frame_padding = Vec2(4.0f, 3.0f);
    float frame_rounding = 0.0f;
    float frame_border_size = 0.0f;
    Vec2 item_spacing = Vec2(8.0f, 4.0f);
    Vec2 item_inner_spacing = Vec2(4.0f, 4.0f);
    Vec2 cell_padding = Vec2(4.0f, 2.0f);
    Vec2 touch_extra_padding = Vec2(0.0f, 0.0f);
    float indent_spacing = 21.0f;
    float columns_min_spacing = 6.0f;
    float scrollbar_size = 14.0f;
    float scrollbar_rounding = 9.0f;
    float grab_min_size = 12.0f;
    float grab_rounding = 0.0f;
    float log_slider_dead_zone = 4.0f;
    float tab_rounding = 4.0f;
    float tab_border_size = 0.0f;
    float tab_min_width_for_close_button = 0.0f;
    Vec2 button_text_align = Vec2(0.5f, 0.5f);
    Vec2 selectable_text_align = Vec2(0.0f, 0.0f);
    Vec2 display_window_padding = Vec2(19.0f, 19.0f);
    Vec2 display_safe_area_padding = Vec2(3.0f, 3.0f);
    float mouse_cursor_scale = 1.0f;
    bool anti_aliased_lines = true;
    bool anti_aliased_lines_use_tex = true;
    bool anti_aliased_fill = true;
    float curve_tessellation_tol = 1.25f;
    float circle_tessellation_max_error = 0.30f;
    std::array<Vec4, Color::COUNT> colors = {};
};

} // namespace pine::gui
