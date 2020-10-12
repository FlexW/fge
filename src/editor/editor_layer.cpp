#include "editor_layer.hpp"
#include "application.hpp"
#include "graphic/imgui.hpp"

namespace Fge
{

void EditorLayer::init() { create_scene_render_view(); }

void EditorLayer::update(float /*delta_time*/) {}

void EditorLayer::fixed_update(float /*frametime*/) {}

void EditorLayer::render() {}

void EditorLayer::imgui_render()
{
  bool show = true;
  ImGui::ShowDemoWindow(&show);
}

void EditorLayer::create_scene_render_view()
{
  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto config_manager  = app->get_config_manager();
  auto window          = graphic_manager->get_window();

  int samples = config_manager->get_config()["window"]["samples"];

  scene_render_view =
      graphic_manager->create_render_view(RenderView::Target::Texture,
                                          window->get_width(),
                                          window->get_height(),
                                          samples);
}

} // namespace Fge
