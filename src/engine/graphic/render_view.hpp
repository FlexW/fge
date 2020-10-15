#pragma once

#include "camera.hpp"
#include "graphic/framebuffer.hpp"
#include "math/math.hpp"
#include "std.hpp"

class GraphicSystem;

namespace Fge
{

class RenderView
{
public:
  enum class Target
  {
    Backbuffer,
    Texture
  };

  RenderView(Target target, uint32_t width, uint32_t height, uint32_t samples);

  void render(const glm::mat4 &projection_mat,
              const Camera &   camera,
              uint32_t         width,
              uint32_t         height,
              uint32_t         samples);

  uint32_t get_target_id() const { return target_id; }

private:
  uint32_t target_id{};

  Target   target;
  uint32_t last_width;
  uint32_t last_height;
  uint32_t last_samples;

  FramebufferConfigRRT target_fb_config{};
  FramebufferConfigRRT ms_resolve_fb_config{};

  std::shared_ptr<Framebuffer> target_fb{};
  std::shared_ptr<Framebuffer> ms_resolve_fb{};

  void recreate_target_framebuffer(uint32_t width,
                                   uint32_t height,
                                   uint32_t samples);
};

} // namespace Fge
