#pragma once

#include <bits/stdint-uintn.h>

namespace fge::gfx
{

enum class key
{
  // clang-format off
  a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
  ctrl_left, ctrl_right, super_left, super_right, shift_left, shift_right,
  arrow_left, arrow_up, arrow_right, arrow_down,
  tab, space,
  f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
  escape
  // clang-format on
};

enum class key_action
{
  press,
  release,
  repeat
};

struct window_resize_event
{
  uint32_t new_window_width;
  uint32_t new_window_height;
};

struct mouse_movement_event
{
  float x;
  float y;
  float x_offset;
  float y_offset;
};

struct mouse_scroll_event
{
  float x_offset;
  float y_offset;
};

struct key_event
{
  key        k;
  key_action action;
};

struct window_close_event
{
};

class window
{
public:
  virtual ~window() = default;

  virtual void flush() = 0;

  virtual void terminate() = 0;

  virtual uint32_t get_window_width() const = 0;

  virtual uint32_t get_window_height() const = 0;

  virtual key_action get_key(key k) const = 0;

  virtual void set_capture_mouse(bool value) = 0;
};

} // namespace fge::gfx
