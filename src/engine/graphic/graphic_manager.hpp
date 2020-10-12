#pragma once

#include "platform/glfw/glfw.hpp"
#include "render_view.hpp"
#include "std.hpp"
#include "window.hpp"

namespace Fge
{

class GraphicManager
{
public:
  void create_window();

  void flush();

  std::shared_ptr<Window> get_window() { return window; }

  std::shared_ptr<RenderView>
  create_render_view(const RenderView::Target target,
                     const uint32_t           width,
                     const uint32_t           height,
                     const uint32_t           samples);

  void terminate();

  void begin_imgui_render();

  void end_imgui_render();

private:
  std::shared_ptr<Window> window{};

  void init_imgui(GLFWwindow *window);

  void terminate_imgui();
};

} // namespace Fge
