#pragma once

#include "graphic/camera.hpp"
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

  void draw(Camera &editor_camera);

private:
  float viewport_x{};
  float viewport_y{};

  float viewport_width  = 100.0f;
  float viewport_height = 100.0f;

  glm::mat4 projection_mat;

  int32_t samples{};

  std::shared_ptr<RenderView> scene_render_view{};

  void recreate_projection_mat(Camera &editor_camera);

  void create_scene_render_view();
};

} // namespace Fge::EditorViews
