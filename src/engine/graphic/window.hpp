#pragma once

#include "event/event.hpp"
#include "std.hpp"

namespace Fge
{

enum class Key
{
  W,
  S,
  A,
  D,
  Esc,
  CtrlLeft,
  F12,

  Unknown

  // TODO: Extend
};

enum class KeyAction
{
  Press,
  Release,
  Repeat,
  Unknown
};

struct WindowCloseEvent : public Event
{
};

class WindowResizeEvent : public Event
{
public:
  unsigned width  = 0;
  unsigned height = 0;
};

class KeyEvent : public Event
{
public:
  Key       key{};
  KeyAction action{};
};

class MouseEvent : public Event
{
public:
  double x{};
  double y{};
  double x_offset{};
  double y_offset{};
};

class Window
{
public:
  virtual ~Window() = default;

  virtual void create_window() = 0;

  virtual void flush() = 0;

  int get_width() { return width; }

  int get_height() { return height; }

  virtual void set_capture_mouse(bool value) = 0;

  virtual void terminate() = 0;

  virtual KeyAction get_key(Key key) = 0;

  virtual double get_mouse_offset_x() const = 0;

  virtual double get_mouse_offset_y() const = 0;

protected:
  int width{};
  int height{};
};

} // namespace Fge
