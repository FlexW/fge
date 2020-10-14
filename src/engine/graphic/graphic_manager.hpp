#pragma once

#include "platform/glfw/glfw.hpp"
#include "render_path.hpp"
#include "render_view.hpp"
#include "renderer.hpp"
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

  void begin_render();

  void end_render();

  std::shared_ptr<Renderer> get_renderer() { return renderer; }

  std::shared_ptr<RenderPath> get_render_path() { return render_path; }

private:
  std::shared_ptr<Window> window{};

  std::shared_ptr<Renderer> renderer{};

  std::shared_ptr<RenderPath> render_path{};

  void init_imgui(GLFWwindow *window);

  void terminate_imgui();
};

} // namespace Fge
