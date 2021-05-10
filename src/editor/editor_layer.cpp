#include <bits/stdint-uintn.h>

#include "application.hpp"
#include "ecs/fwd.hpp"
#include "editor_layer.hpp"
#include "fmt/core.h"
#include "gfx/camera.hpp"
#include "gfx/command_bucket.hpp"
#include "gfx/graphic_backend.hpp"
#include "gfx/graphic_backend_types.hpp"
#include "gfx/window.hpp"
#include "layer.hpp"
#include "scn/components.hpp"
#include "util/filesystem.hpp"

using namespace std::placeholders;

namespace fge
{

float cube_vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

glm::vec3 cube_positions[] = {glm::vec3(0.0f, 0.0f, 0.0f),
                              glm::vec3(2.0f, 5.0f, -15.0f),
                              glm::vec3(-1.5f, -2.2f, -2.5f),
                              glm::vec3(-3.8f, -2.0f, -12.3f),
                              glm::vec3(2.4f, -0.4f, -3.5f),
                              glm::vec3(-1.7f, 3.0f, -7.5f),
                              glm::vec3(1.3f, -2.0f, -2.5f),
                              glm::vec3(1.5f, 2.0f, -2.5f),
                              glm::vec3(1.5f, 0.2f, -1.5f),
                              glm::vec3(-1.3f, 1.0f, -1.5f)};

editor_layer::editor_layer(application *app) : layer(app) {}

void editor_layer::init()
{
  app->get_renderer()->get_renderer_window()->set_capture_mouse(true);

  // app->get_application_broadcast()->subscribe<mouse_movement_event>(
  //     [](const mouse_movement_event &event) {
  //       std::cout << event.x_offset << std::endl;
  //       return false;
  //     });
  app->get_application_broadcast()->subscribe<gfx::mouse_movement_event>(
      std::bind(&editor_layer::on_mouse_movement, this, _1));
  app->get_application_broadcast()->subscribe<gfx::mouse_scroll_event>(
      std::bind(&editor_layer::on_mouse_scroll, this, _1));

  // Init resources
  const auto vertex_shader_source = fs::load_text_file("/shaders/shader.vert");
  const auto fragment_shader_source =
      fs::load_text_file("/shaders/shader.frag");

  shader_program =
      gfx::graphic_backend::create_shader_program(vertex_shader_source,
                                                  fragment_shader_source);

  gfx::vertex_buffer_layout cube_vb_layout;
  cube_vb_layout.add_float(3);
  cube_vb_layout.add_float(2);

  gfx::memory cube_vb_memory{};
  cube_vb_memory.data = cube_vertices;
  cube_vb_memory.size = sizeof(cube_vertices);

  cube_vertex_buffer =
      gfx::graphic_backend::create_vertex_buffer(cube_vb_memory,
                                                 cube_vb_layout);

  const auto container_image = fs::load_image("/textures/container.jpg");

  gfx::texture_info container_tex_info{};
  container_tex_info.type                 = gfx::texture_type::two_d;
  container_tex_info.width                = container_image.width;
  container_tex_info.height               = container_image.height;
  container_tex_info.wrap_u               = gfx::wrap_mode::repeat;
  container_tex_info.wrap_v               = gfx::wrap_mode::repeat;
  container_tex_info.minification_filter  = gfx::filter_mode::linear;
  container_tex_info.magnification_filter = gfx::filter_mode::linear;
  container_tex_info.internal_format      = gfx::pixel_internal_format::rgb;
  container_tex_info.source_format        = gfx::pixel_format::rgb;
  container_tex_info.source_type          = gfx::pixel_type::ubyte;
  container_tex_info.generate_mipmap      = true;

  container_texture = gfx::graphic_backend::create_texture(container_image.data,
                                                           container_tex_info);

  const auto awesomeface_image = fs::load_image("/textures/awesomeface.png");

  gfx::texture_info awesomeface_tex_info{};
  awesomeface_tex_info.type                 = gfx::texture_type::two_d;
  awesomeface_tex_info.width                = awesomeface_image.width;
  awesomeface_tex_info.height               = awesomeface_image.height;
  awesomeface_tex_info.wrap_u               = gfx::wrap_mode::repeat;
  awesomeface_tex_info.wrap_v               = gfx::wrap_mode::repeat;
  awesomeface_tex_info.minification_filter  = gfx::filter_mode::linear;
  awesomeface_tex_info.magnification_filter = gfx::filter_mode::linear;
  awesomeface_tex_info.internal_format      = gfx::pixel_internal_format::rgb;
  awesomeface_tex_info.source_format        = gfx::pixel_format::rgba;
  awesomeface_tex_info.source_type          = gfx::pixel_type::ubyte;
  awesomeface_tex_info.generate_mipmap      = true;

  awesomeface_texture =
      gfx::graphic_backend::create_texture(awesomeface_image.data,
                                           awesomeface_tex_info);

  auto scene = std::make_unique<scn::scene>(&editor_layer_broadcast);

  ecs::registry scene_registry;
  const auto    cube1 = scene_registry.create();

  auto &        cube1_mesh_component =
      scene_registry.emplace<scn::mesh_component>(cube1);
  cube1_mesh_component.vertex_buffer = cube_vertex_buffer;
  cube1_mesh_component.material      = ;

  auto &cube1_transform_component =
      scene_registry.emplace<scn::transform_component>(cube1);
  cube1_transform_component.position = ;
  cube1_transform_component.rotation = ;

  scene->set_registry(std::move(scene_registry));
  app->set_scene(std::move(scene));
}

void editor_layer::update(float delta_time)
{
  const auto renderer       = app->get_renderer();
  const auto window         = renderer->get_renderer_window();

  if (window->get_key(gfx::key::w) == gfx::key_action::press)
  {
    camera.move(gfx::camera_movement::forward, delta_time);
  }
  else if (window->get_key(gfx::key::s) == gfx::key_action::press)
  {
    camera.move(gfx::camera_movement::backward, delta_time);
  }
  else if (window->get_key(gfx::key::a) == gfx::key_action::press)
  {
    camera.move(gfx::camera_movement::left, delta_time);
  }
  else if (window->get_key(gfx::key::d) == gfx::key_action::press)
  {
    camera.move(gfx::camera_movement::right, delta_time);
  }
  else if (window->get_key(gfx::key::escape) == gfx::key_action::press)
  {
    app->close();
  }

  const auto command_bucket = renderer->create_command_bucket();
  command_bucket->set_clear_color(0.2f, 0.3f, 0.3f, 1.0f);
  command_bucket->set_state(gfx::render_state::clear_color |
                            gfx::render_state::clear_depth);
  command_bucket->set_viewport(0,
                               0,
                               window->get_window_width(),
                               window->get_window_height());

  glm::mat4 projection =
      glm::perspective(glm::radians(camera.get_zoom()),
                       static_cast<float>(window->get_window_width()) /
                           static_cast<float>(window->get_window_height()),
                       0.1f,
                       100.0f);
  glm::mat4 view = camera.get_view_matrix();

  for (uint32_t i = 0; i < 10; ++i)
  {
    auto model       = glm::mat4(1.0f);
    model            = glm::translate(model, cube_positions[i]);
    const auto angle = 20.0f * i;
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

    auto set_proj_mat_command =
        command_bucket->new_command<gfx::set_uniform_command<glm::mat4>>();
    set_proj_mat_command->shader = shader_program;
    set_proj_mat_command->type   = gfx::uniform_type::mat4;
    set_proj_mat_command->name   = "projection";
    set_proj_mat_command->data   = projection;

    auto set_view_mat_command =
        command_bucket->append_command<gfx::set_uniform_command<glm::mat4>>(
            set_proj_mat_command);
    set_view_mat_command->shader = shader_program;
    set_view_mat_command->type   = gfx::uniform_type::mat4;
    set_view_mat_command->name   = "view";
    set_view_mat_command->data   = view;

    auto set_model_mat_command =
        command_bucket->append_command<gfx::set_uniform_command<glm::mat4>>(
            set_view_mat_command);
    set_model_mat_command->shader = shader_program;
    set_model_mat_command->type   = gfx::uniform_type::mat4;
    set_model_mat_command->name   = "model";
    set_model_mat_command->data   = model;

    auto set_tex2d1_command =
        command_bucket
            ->append_command<gfx::set_uniform_command<gfx::texture_handle>>(
                set_model_mat_command);
    set_tex2d1_command->shader = shader_program;
    set_tex2d1_command->type   = gfx::uniform_type::texture2d;
    set_tex2d1_command->name   = "texture1";
    set_tex2d1_command->data   = container_texture;

    auto set_tex2d2_command =
        command_bucket
            ->append_command<gfx::set_uniform_command<gfx::texture_handle>>(
                set_tex2d1_command);
    set_tex2d2_command->shader = shader_program;
    set_tex2d2_command->type   = gfx::uniform_type::texture2d;
    set_tex2d2_command->name   = "texture2";
    set_tex2d2_command->data   = awesomeface_texture;

    auto draw_cube_command =
        command_bucket->append_command<gfx::draw_command>(set_tex2d2_command);
    draw_cube_command->vertices   = cube_vertex_buffer;
    draw_cube_command->shader     = shader_program;
    draw_cube_command->depth_test = true;
  }
}

void editor_layer::terminate() {}

bool editor_layer::on_mouse_movement(const gfx::mouse_movement_event &event)
{
  camera.rotate(event.x_offset, event.y_offset);
  return false;
}

bool editor_layer::on_mouse_scroll(const gfx::mouse_scroll_event &event)
{
  camera.scroll(event.y_offset);
  return false;
}

} // namespace fge
