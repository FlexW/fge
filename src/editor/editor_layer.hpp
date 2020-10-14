#pragma once

#include "graphic/camera.hpp"
#include "graphic/render_view.hpp"
#include "graphic/window.hpp"
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
  std::shared_ptr<RenderView> scene_render_view{};

  int32_t window_width    = 0;
  int32_t window_height   = 0;
  int32_t samples         = 0;
  int32_t viewport_width  = 0;
  int32_t viewport_height = 0;

  glm::mat4 projection_mat{};

  bool   move_camera = false;
  Camera editor_camera;

  void create_scene_render_view();

  void recreate_projection_mat();
};

} // namespace Fge
