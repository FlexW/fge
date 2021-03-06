﻿#include "editor_layer.hpp"
#include "application.hpp"
#include "glm/ext/vector_float3.hpp"
#include "graphic/camera.hpp"
#include "graphic/imgui.hpp"
#include "graphic/render_info.hpp"
#include "graphic/window.hpp"
#include "imgui_views/dockspace.hpp"
#include "log/log.hpp"
#include "scene/actor.hpp"
#include "scene/components/components.hpp"
#include "scene/components/follow_camera_component.hpp"
#include "scene/components/lua_script_component.hpp"
#include "scene/components/point_light_component.hpp"
#include "scene/components/sphere_rigid_body_component.hpp"
#include "scene/scene.hpp"
#include "util/assert.hpp"

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

  // Init imgui views
  scene_viewport.init();

  dockspace.signal_save_scene.subscribe(this, &EditorLayer::save_scene);
  dockspace.signal_load_scene.subscribe(this, &EditorLayer::load_scene);

  // Subscribe events
  event_manager->subscribe(this, &EditorLayer::on_window_resize_event, 0);
  event_manager->subscribe(this, &EditorLayer::on_key_event, 0);
  event_manager->subscribe(this, &EditorLayer::on_mouse_event, 0);

  create_grid(50);

  // ----------------------------- BEGIN TEST SCENE ---------------------------

  auto scene     = std::make_shared<Scene>();
  auto actor     = scene->add_actor<Actor>();
  auto mesh_comp = actor->add_component<MeshComponent>();
  mesh_comp->set_mesh_from_file("character.dae");
  auto script_comp = actor->add_component<LuaScriptComponent>();
  script_comp->set_script_from_file("test_script.lua");

  actor = scene->add_actor<Actor>();
  actor->set_position(glm::vec3(10.0f, 0.0f, 0.0f));
  auto skinned_mesh_comp = actor->add_component<SkinnedMeshComponent>();
  skinned_mesh_comp->set_mesh_from_file("character.dae");
  skinned_mesh_comp->play_animation_endless("Animation 0");
  auto follow_camera_comp = actor->add_component<FollowCameraComponent>();
  script_comp             = actor->add_component<LuaScriptComponent>();
  script_comp->set_script_from_file("move_character.lua");

  actor = scene->add_actor<Actor>();
  actor->set_scale(glm::vec3(10.0f));
  mesh_comp = actor->add_component<MeshComponent>();
  mesh_comp->set_mesh_from_file("plane.dae");
  auto box_rigid_body_comp = actor->add_component<BoxRigidBodyComponent>();
  box_rigid_body_comp->set_half_extents(10.0f, 0.01f, 10.0f);
  box_rigid_body_comp->set_mass(0.0f);

  actor = scene->add_actor<Actor>();
  actor->set_position(glm::vec3(0.9f, 0.0f, 0.0f));
  actor->set_rotation_euler(glm::vec3(0.2f, 0.1f, 0.0f));
  mesh_comp = actor->add_component<MeshComponent>();
  mesh_comp->set_mesh_from_file("cube.dae");
  box_rigid_body_comp = actor->add_component<BoxRigidBodyComponent>();
  box_rigid_body_comp->set_half_extents(1.0f, 1.0f, 1.0f);
  box_rigid_body_comp->set_mass(0.0f);

  actor = scene->add_actor<Actor>();
  actor->set_position(glm::vec3(0.0f, 20.0f, 0.0f));
  mesh_comp = actor->add_component<MeshComponent>();
  mesh_comp->set_mesh_from_file("sphere.dae");
  auto sphere_rigid_body_comp =
      actor->add_component<SphereRigidBodyComponent>();
  sphere_rigid_body_comp->set_radius(1.0f);
  sphere_rigid_body_comp->set_mass(1.0f);

  // actor = scene->add_actor<Actor>();
  // actor->set_position(glm::vec3(3.0f, 0.0f, 0.0f));
  // mesh_comp = actor->add_component<MeshComponent>();
  // mesh_comp->set_mesh_from_file("bunny.obj");

  actor = scene->add_actor<Actor>();
  actor->set_position(glm::vec3(0.0f, 10.0f, 0.0f));
  auto point_light_comp = actor->add_component<PointLightComponent>();

  // actor               = scene->add_actor<Actor>();
  // auto dir_light_comp = actor->add_component<DirectionalLightComponent>();

  // script_comp = actor->add_component<LuaScriptComponent>();
  // script_comp->set_script_from_file("test_script.lua");

  auto scene_manager = app->get_scene_manager();
  scene_manager->set_scene(scene);

  // ----------------------------- END TEST SCENE ---------------------------
}

void EditorLayer::do_camera_movement(float delta_time)
{
  if (move_camera)
  {
    auto window =
        Application::get_instance()->get_graphic_manager()->get_window();

    if (window->get_key(Key::W) == KeyAction::Press)
    {
      editor_camera.process_movement(CameraMovement::Forward, delta_time);
    }
    else if (window->get_key(Key::S) == KeyAction::Press)
    {
      editor_camera.process_movement(CameraMovement::Backward, delta_time);
    }
    else if (window->get_key(Key::A) == KeyAction::Press)
    {
      editor_camera.process_movement(CameraMovement::Left, delta_time);
    }
    else if (window->get_key(Key::D) == KeyAction::Press)
    {
      editor_camera.process_movement(CameraMovement::Right, delta_time);
    }
  }
}

void EditorLayer::update(float delta_time)
{
  handle_grid_registration();
  do_camera_movement(delta_time);
}

void EditorLayer::fixed_update(float /*frametime*/) {}

void EditorLayer::render() {}

void EditorLayer::imgui_render()
{
  dockspace.draw();

  auto app                = Application::get_instance();
  auto camera_controller  = app->get_graphic_manager()->get_camera_controller();
  const auto &camera_info = camera_controller.get_camera_info();

  scene_viewport.draw(camera_info);

  // bool show = true;
  // ImGui::ShowDemoWindow(&show);
}

bool EditorLayer::on_window_resize_event(const WindowResizeEvent *const event)
{
  window_width  = event->width;
  window_height = event->height;

  viewport_width  = window_width;
  viewport_height = window_height;

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

void EditorLayer::handle_grid_registration()
{
  if (show_grid)
  {
    register_grid();
  }
  else
  {
    unregister_grid();
  }
}

void EditorLayer::create_grid(int32_t size)
{
  FGE_ASSERT(size > 1);

  if (size % 2 == 1)
  {
    --size;
  }

  unregister_grid();

  grid             = std::make_unique<Grid>(size);
  grid_render_info = grid->create_render_info();
}

void EditorLayer::unregister_grid()
{
  if (grid_registered)
  {
    trace("EditorLayer", "Unregister grid");

    auto app      = Application::get_instance();
    auto renderer = app->get_graphic_manager()->get_renderer();

    renderer->unregister_renderable(grid_render_info);
    grid_registered = false;
  }
}

void EditorLayer::register_grid()
{
  if (!grid_registered)
  {
    trace("EditorLayer", "Register grid");

    auto app      = Application::get_instance();
    auto renderer = app->get_graphic_manager()->get_renderer();

    renderer->register_renderable(grid_render_info);
    grid_registered = true;
  }
}

bool EditorLayer::save_scene(
    const EditorViews::SignalSaveScene *const /*signal*/)
{
  std::cout << "Save scene" << std::endl;
  return false;
}

bool EditorLayer::load_scene(
    const EditorViews::SignalLoadScene *const /*signal*/)
{
  std::cout << "Load scene" << std::endl;
  return false;
}

} // namespace Fge
