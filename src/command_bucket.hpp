#pragma once

#include <cstdint>
#include <vector>

#include "command_packet.hpp"
#include "graphic_backend.hpp"

namespace fge::gfx
{

constexpr uint64_t MAX_PACKET_COUNT = 2000;

using key = uint64_t;

void init_buckets();

void start_frame();

class command_bucket
{
public:
  static command_bucket *create();

  static std::vector<command_bucket *> get_buckets();

  template <typename TCommand> TCommand *new_command()
  {
    auto packet = command_packet::create<TCommand>();

    {
      FGE_ASSERT(packet_count < MAX_PACKET_COUNT);
      // TODO: Add locking
      packets[packet_count] = packet;
      ++packet_count;
    }

    command_packet::store_next_packet(packet, nullptr);
    command_packet::store_dispatch_function(packet,
                                            TCommand::DISPATCH_FUNCTION);

    return command_packet::get_command<TCommand>(packet);
  }

  template <typename TCommand, typename TPrevCommand>
  TCommand *append_command(TPrevCommand *previous_command)
  {
    auto packet = command_packet::create<TCommand>();

    // Append this command to the given one
    command_packet::store_next_packet<TPrevCommand>(previous_command, packet);

    command_packet::store_next_packet(packet, nullptr);
    command_packet::store_dispatch_function(packet,
                                            TCommand::DISPATCH_FUNCTION);

    return command_packet::get_command<TCommand>(packet);
  }

  void submit() const;

  void set_state(render_state state);

  void set_clear_color(float red, float green, float blue, float alpha);

  void clear();

private:
  static std::vector<command_bucket *> command_buckets;

  key *  keys[MAX_PACKET_COUNT];
  void * packets[MAX_PACKET_COUNT];

  uint64_t packet_count = 0;

  render_state state;

  struct
  {
    float red;
    float green;
    float blue;
    float alpha;
  } clear_color;

  command_bucket();
};

} // namespace fge::gfx::command_bucket
