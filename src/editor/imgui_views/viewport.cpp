#include "viewport.hpp"
#include "application.hpp"
#include "graphic/camera.hpp"
#include "graphic/imgui.hpp"

namespace Fge::EditorViews
{
SceneViewport::SceneViewport() {}

void SceneViewport::init() { create_scene_render_view(); }

void SceneViewport::draw(const CameraInfo &camera_info)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Viewport");

  const auto viewport_pos  = ImGui::GetWindowPos();
  viewport_x               = viewport_pos.x;
  viewport_y               = viewport_pos.y;
  const auto viewport_size = ImGui::GetContentRegionAvail();
  if (viewport_size.x != viewport_width || viewport_size.y != viewport_height)
  {
    viewport_width  = viewport_size.x;
    viewport_height = viewport_size.y;

    recreate_projection_mat(camera_info);
  }

  scene_render_view->render(projection_mat,
                            camera_info,
                            viewport_width,
                            viewport_height,
                            samples);

  ImGui::Image(reinterpret_cast<void *>(scene_render_view->get_target_id()),
               ImVec2(viewport_width, viewport_height),
               ImVec2(0, 1),
               ImVec2(1, 0));

  ImGui::End();
  ImGui::PopStyleVar();
}

void SceneViewport::recreate_projection_mat(const CameraInfo &camera_info)
{
  projection_mat =
      glm::perspective(camera_info.zoom,
                       static_cast<float>(viewport_width) / viewport_height,
                       0.1f,
                       500.0f);
}

void SceneViewport::create_scene_render_view()
{
  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto config_manager  = app->get_config_manager();

  samples = config_manager->get_config()["window"]["samples"];

  scene_render_view =
      graphic_manager->create_render_view(RenderView::Target::Texture,
                                          viewport_width,
                                          viewport_height,
                                          samples);
}

} // namespace Fge::EditorViews
