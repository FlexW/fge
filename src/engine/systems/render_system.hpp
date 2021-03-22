#pragma once

#include "scn/system.hpp"

namespace fge
{

class render_system : public scn::system
{
public:
  void update() override;
};
}
