#include <GLFW/glfw3.h>

#include "time.hpp"

namespace fge
{

void delta_time::start()
{
  // FIXME: Make independent of glfw
  last_frame = glfwGetTime();
}

void delta_time::update()
{
  // FIXME: Make independent of glfw
  const auto current_frame = glfwGetTime();
  current_delta_time       = current_frame - last_frame;
  last_frame               = current_frame;
}

float delta_time::get() const { return current_delta_time; }

} // namespace fge
