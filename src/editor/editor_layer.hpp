#pragma once

#include "graphic/camera.hpp"
#include "graphic/grid.hpp"
#include "graphic/render_info.hpp"
#include "graphic/render_view.hpp"
#include "graphic/window.hpp"
#include "imgui_views/dockspace.hpp"
#include "imgui_views/viewport.hpp"
#include "layer.hpp"

namespace Fge
{

class EditorLayer : public Layer
{
public:
  virtual void init();

  virtual void update(float delta_time);

  virtual void fixed_update(float frametime);

  virtual void render();

  virtual void imgui_render();

  bool on_window_resize_event(const WindowResizeEvent *const event);

  bool on_key_event(const KeyEvent *const event);

  bool on_mouse_event(const MouseEvent *const event);

private:
  int32_t window_width    = 0;
  int32_t window_height   = 0;
  int32_t samples         = 0;
  int32_t viewport_width  = 0;
  int32_t viewport_height = 0;

  bool   move_camera = false;
  Camera editor_camera;

  // ImGui Views
  EditorViews::DockSpace     dockspace;
  EditorViews::SceneViewport scene_viewport;

  std::unique_ptr<Grid>       grid{};
  bool                        grid_registered = false;
  bool                        show_grid       = true;
  std::shared_ptr<RenderInfo> grid_render_info{};

  void create_grid(int32_t size);

  void handle_grid_registration();

  void unregister_grid();

  void register_grid();

  void do_camera_movement(float delta_time);
};

} // namespace Fge
