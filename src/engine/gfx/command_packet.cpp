#include "command_packet.hpp"

namespace fge::gfx::command_packet
{

constexpr size_t buffer_size = 50000;
fge::linear_allocator allocator;

packet *get_next_packet(packet p)
{
  return reinterpret_cast<packet *>(reinterpret_cast<char *>(p) +
                                    OFFSET_NEXT_COMMAND_PACKET);
}

dispatch_function *get_dispatch_function(packet p)
{
  return reinterpret_cast<dispatch_function *>(
      reinterpret_cast<char *>(p) + OFFSET_BACKEND_DISPATCH_FUNCTION);
}

void store_next_packet(packet p, packet next_packet)
{
  *get_next_packet(p) = next_packet;
}

void store_dispatch_function(packet p, dispatch_function dispatch_function)
{
  *get_dispatch_function(p) = dispatch_function;
}

packet load_next_packet(const packet p) { return *get_next_packet(p); }

dispatch_function load_dispatch_function(const packet p)
{
  return *get_dispatch_function(p);
}

const void *load_command(const packet p)
{
  return reinterpret_cast<char *>(p) + OFFSET_COMMAND;
}

void init_packets()
{
  auto buffer      = std::malloc(buffer_size);
  allocator        = linear_allocator(buffer, buffer_size);
}

void start_frame() { allocator.reset(); }

} // namespace fge::gfx::command_packet