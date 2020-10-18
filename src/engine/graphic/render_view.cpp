#include "render_view.hpp"
#include "application.hpp"
#include "graphic/framebuffer.hpp"
#include "graphic/graphic_manager.hpp"
#include "graphic/renderbuffer.hpp"
#include "graphic/texture.hpp"
#include "log/log.hpp"
#include "util/assert.hpp"

namespace Fge
{

RenderView::RenderView(Target   target,
                       uint32_t width,
                       uint32_t height,
                       uint32_t samples)
    : target(target),
      last_width(width),
      last_height(height),
      last_samples(samples)
{
  if (target == Target::Texture)
  {
    recreate_target_framebuffer(width, height, samples);
  }
}

void RenderView::render(const glm::mat4 & projection_mat,
                        const CameraInfo &camera_info,
                        uint32_t          width,
                        uint32_t          height,
                        uint32_t          samples)
{
  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto renderer        = graphic_manager->get_renderer();
  auto render_path     = graphic_manager->get_render_path();

  if (target == Target::Texture)
  {
    // Check if target needs to be recreated
    if (width != last_width || height != last_height || samples != last_samples)
    {
      recreate_target_framebuffer(width, height, samples);
      last_width   = width;
      last_height  = height;
      last_samples = samples;
    }

    target_fb->bind();
    render_path->render(projection_mat, camera_info, width, height);

    if (last_samples > 0)
    {
      target_fb->bind(FramebufferBindMode::Read);
      ms_resolve_fb->bind(FramebufferBindMode::Draw);

      renderer->blit_framebuffer(0,
                                 0,
                                 last_width,
                                 last_height,
                                 0,
                                 0,
                                 last_width,
                                 last_height);

      target_fb->unbind(FramebufferBindMode::Read);
      ms_resolve_fb->unbind(FramebufferBindMode::Draw);
    }
    target_fb->unbind();

    return;
  }

  render_path->render(projection_mat, camera_info, width, height);
}

void RenderView::recreate_target_framebuffer(uint32_t width,
                                             uint32_t height,
                                             uint32_t samples)
{
  auto app      = Application::get_instance();
  auto renderer = app->get_graphic_manager()->get_renderer();

  trace("RenderView",
        "Recreate target framebuffer with width: {}, height: {}, samples: {}",
        width,
        height,
        samples);

  RenderbufferConfig depth_rb_config{};
  depth_rb_config.width   = width;
  depth_rb_config.height  = height;
  depth_rb_config.samples = samples;
  depth_rb_config.format  = RenderBufferFormat::DepthComponent32;

  Texture2DConfig color_tex_config{};
  color_tex_config.width   = width;
  color_tex_config.height  = height;
  color_tex_config.samples = samples;
  color_tex_config.format  = ImageFormat::Rgba;

  target_fb_config.depth_attachment =
      renderer->create_renderbuffer(depth_rb_config);
  target_fb_config.color_attachment =
      renderer->create_texture2d(color_tex_config);

  target_fb = renderer->create_framebuffer_rrt(target_fb_config);

  target_id = target_fb_config.color_attachment->get_id();

  if (samples > 0)
  {
    Texture2DConfig color_tex_config{};
    color_tex_config.width   = width;
    color_tex_config.height  = height;
    color_tex_config.samples = 0;
    color_tex_config.format  = ImageFormat::Rgba;

    ms_resolve_fb_config.color_attachment =
        renderer->create_texture2d(color_tex_config);

    ms_resolve_fb = renderer->create_framebuffer_rrt(ms_resolve_fb_config);

    target_id = ms_resolve_fb_config.color_attachment->get_id();
  }
}

} // namespace Fge
