#pragma once

#include "graphic/camera_controller.hpp"
#include "graphic/render_view.hpp"
#include "math/math.hpp"
#include "std.hpp"

namespace Fge::EditorViews
{

class SceneViewport
{
public:
  SceneViewport();

  void init();

  void draw(const CameraInfo &camera_info);

private:
  float viewport_x{};
  float viewport_y{};

  float viewport_width  = 100.0f;
  float viewport_height = 100.0f;

  glm::mat4 projection_mat;

  int32_t samples{};

  std::shared_ptr<RenderView> scene_render_view{};

  void recreate_projection_mat(const CameraInfo &camera_info);

  void create_scene_render_view();
};

} // namespace Fge::EditorViews
