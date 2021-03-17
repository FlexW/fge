#pragma once

#include <cstdint>

#include "ecs/entity.hpp"
#include "registry.hpp"

namespace fge::ecs
{

using entity = basic_entity<std::uint32_t, std::uint32_t>;

using registry = basic_registry<entity>;
}
