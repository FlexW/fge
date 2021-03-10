#include "command_bucket.hpp"
#include "command_packet.hpp"

namespace fge::gfx
{

std::vector<command_bucket *> command_bucket::command_buckets;

command_bucket::command_bucket() = default;

command_bucket *command_bucket::create()
{
  auto bucket = new command_bucket;
  command_buckets.push_back(bucket);
  return bucket;
}

std::vector<command_bucket *> command_bucket::get_buckets()
{
  return command_buckets;
}

static void submit_packet(const command_packet::packet packet)
{
  const auto dispatch_function = command_packet::load_dispatch_function(packet);
  const auto command           = command_packet::load_command(packet);

  dispatch_function(command);
}

void command_bucket::submit() const
{
  set_render_state(state);
  gfx::set_clear_color(clear_color.red,
                       clear_color.green,
                       clear_color.blue,
                       clear_color.alpha);
  gfx::set_viewport(viewport.x, viewport.y, viewport.width, viewport.height);

  if (fb_handle.has_value())
  {
    gfx::bind_framebuffer(fb_handle.value());
  }

  for (size_t i = 0; i < packet_count; ++i)
  {
    auto packet = packets[i];
    do
    {
      submit_packet(packet);
      packet = command_packet::load_next_packet(packet);
    } while (packet != nullptr);
  }

  if (fb_handle.has_value())
  {
    gfx::bind_default_framebuffer();
  }
}

void command_bucket::clear() { packet_count = 0; }

void command_bucket::set_state(render_state state) { this->state = state; }

void command_bucket::set_clear_color(float red,
                                     float green,
                                     float blue,
                                     float alpha)
{

  clear_color = {red, green, blue, alpha};
}

void command_bucket::set_viewport(uint32_t x,
                                  uint32_t y,
                                  uint32_t width,
                                  uint32_t height)
{
  viewport.x      = x;
  viewport.y      = y;
  viewport.width  = width;
  viewport.height = height;
}

void command_bucket::set_framebuffer(framebuffer_handle handle)
{
  fb_handle = handle;
}

void start_frame() { command_packet::start_frame(); }

void init_buckets() { command_packet::init_packets(); }

} // namespace fge::gfx
