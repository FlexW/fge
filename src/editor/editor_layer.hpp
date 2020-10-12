#pragma once

#include "graphic/render_view.hpp"
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

private:
  std::shared_ptr<RenderView> scene_render_view{};

  void create_scene_render_view();
};

} // namespace Fge
