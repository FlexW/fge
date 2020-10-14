#include "editor_layer.hpp"
#include "application.hpp"
#include "graphic/camera.hpp"
#include "graphic/imgui.hpp"
#include "graphic/window.hpp"
#include "scene/actor.hpp"
#include "scene/components/components.hpp"
#include "scene/scene.hpp"

namespace Fge
{

void EditorLayer::init()
{
  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto config_manager  = app->get_config_manager();
  auto event_manager   = app->get_event_manager();
  auto window          = graphic_manager->get_window();

  samples         = config_manager->get_config()["window"]["samples"];
  viewport_width  = window->get_width();
  viewport_height = window->get_height();
  window_width    = window->get_width();
  window_height   = window->get_height();

  recreate_projection_mat();
  create_scene_render_view();

  // Subscribe events
  event_manager->subscribe(this, &EditorLayer::on_window_resize_event, 0);
  event_manager->subscribe(this, &EditorLayer::on_key_event, 0);
  event_manager->subscribe(this, &EditorLayer::on_mouse_event, 0);

  // ----------------------------- BEGIN TEST SCENE ---------------------------

  auto scene     = std::make_shared<Scene>();
  auto actor     = scene->add_actor<Actor>();
  auto mesh_comp = actor->add_component<MeshComponent>();
  // mesh_comp->set_mesh_from_file("bunny.obj");
  mesh_comp->set_mesh_from_file("character.dae");

  auto scene_manager = app->get_scene_manager();
  scene_manager->set_scene(scene);

  // ----------------------------- END TEST SCENE ---------------------------
}

void EditorLayer::update(float /*delta_time*/) {}

void EditorLayer::fixed_update(float /*frametime*/) {}

void EditorLayer::render()
{
  scene_render_view->render(projection_mat,
                            editor_camera,
                            viewport_width,
                            viewport_height,
                            samples);
}

void EditorLayer::imgui_render()
{
  bool show = true;
  ImGui::ShowDemoWindow(&show);
}

void EditorLayer::create_scene_render_view()
{
  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();

  scene_render_view =
      graphic_manager->create_render_view(RenderView::Target::Backbuffer,
                                          viewport_width,
                                          viewport_height,
                                          samples);
}

void EditorLayer::recreate_projection_mat()
{
  projection_mat =
      glm::perspective(editor_camera.get_zoom(),
                       static_cast<float>(viewport_width) / viewport_height,
                       0.1f,
                       500.0f);
}

bool EditorLayer::on_window_resize_event(const WindowResizeEvent *const event)
{
  window_width  = event->width;
  window_height = event->height;

  viewport_width  = window_width;
  viewport_height = window_height;

  recreate_projection_mat();

  return false;
}

bool EditorLayer::on_key_event(const KeyEvent *const event)
{
  if (event->key == Key::CtrlLeft && event->action == KeyAction::Press)
  {
    move_camera = true;

    auto app = Application::get_instance();
    app->get_graphic_manager()->get_window()->set_capture_mouse(true);
  }
  else if (event->key == Key::CtrlLeft && event->action == KeyAction::Release)
  {
    move_camera = false;

    auto app = Application::get_instance();
    app->get_graphic_manager()->get_window()->set_capture_mouse(false);
  }

  if (move_camera)
  {
    if (event->key == Key::W)
    {
      editor_camera.process_movement(CameraMovement::Forward, 1.0f);
    }
    else if (event->key == Key::S)
    {
      editor_camera.process_movement(CameraMovement::Backward, 1.0f);
    }
    else if (event->key == Key::A)
    {
      editor_camera.process_movement(CameraMovement::Left, 1.0f);
    }
    else if (event->key == Key::D)
    {
      editor_camera.process_movement(CameraMovement::Right, 1.0f);
    }
  }

  return false;
}

bool EditorLayer::on_mouse_event(const MouseEvent *const event)
{

  if (move_camera)
  {
    editor_camera.process_rotation(event->x_offset, event->y_offset);
  }

  return false;
}

} // namespace Fge
