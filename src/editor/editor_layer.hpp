#pragma once

#include "broadcast.hpp"
#include "gfx/camera.hpp"
#include "gfx/graphic_backend_types.hpp"
#include "gfx/window.hpp"
#include "layer.hpp"

namespace fge
{

class editor_layer : public layer
{
public:
  editor_layer(application *app);

  void init() override;

  void update(float delta_time) override;

  void terminate() override;

private:
  gfx::shader_program_handle shader_program;
  gfx::vertex_buffer_handle  cube_vertex_buffer;
  gfx::texture_handle        container_texture;
  gfx::texture_handle        awesomeface_texture;

  gfx::camera camera;

  broadcast editor_layer_broadcast;

  bool on_mouse_movement(const gfx::mouse_movement_event &event);

  bool on_mouse_scroll(const gfx::mouse_scroll_event &event);
};

} // namespace fge
