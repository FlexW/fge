#pragma once

#include <cstddef>

#include "commands.hpp"
#include "mem/linear_allocator.hpp"

namespace fge::gfx::command_packet
{

extern linear_allocator allocator;

using packet = void *;

static const size_t OFFSET_NEXT_COMMAND_PACKET = 0u;
static const size_t OFFSET_BACKEND_DISPATCH_FUNCTION =
    OFFSET_NEXT_COMMAND_PACKET + sizeof(packet);
static const size_t OFFSET_COMMAND =
    OFFSET_BACKEND_DISPATCH_FUNCTION + sizeof(dispatch_function);

template <typename T> size_t get_size() { return OFFSET_COMMAND + sizeof(T); }

template <typename T> packet create()
{
  // return ::operator new(get_size<T>());
  return allocator.allocate(get_size<T>(), 64);
}

template <typename T> packet *get_next_packet(T *command)
{
  return reinterpret_cast<packet *>(reinterpret_cast<char *>(command) -
                                    OFFSET_COMMAND +
                                    OFFSET_NEXT_COMMAND_PACKET);
}

packet *get_next_packet(packet p);

dispatch_function *get_dispatch_function(packet p);

template <typename T> T *get_command(packet p)
{
  return reinterpret_cast<T *>(reinterpret_cast<char *>(p) + OFFSET_COMMAND);
}

void store_next_packet(packet p, packet next_packet);

template <typename T> void store_next_packet(T *command, packet next_packet)
{
  *get_next_packet<T>(command) = next_packet;
}

void store_dispatch_function(packet p, dispatch_function dispatch_function);

packet load_next_packet(const packet p);

dispatch_function load_dispatch_function(const packet p);

const void *load_command(const packet p);

void init_packets();

void start_frame();

} // namespace fge::gfx::command_packet
