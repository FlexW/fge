#include "command_bucket.hpp"

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
  const auto command = command_packet::load_command(packet);

  dispatch_function(command);
}

void command_bucket::submit() const
{
  for (size_t i = 0; i < packet_count; ++i)
  {
    auto packet = packets[i];
    do
    {
      submit_packet(packet);
      packet = command_packet::load_next_packet(packet);
    } while (packet != nullptr);
  }
}

void command_bucket::clear() { packet_count = 0; }

} // namespace fge::gfx
