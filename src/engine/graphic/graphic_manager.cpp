#include "graphic_manager.hpp"
#include "platform/glfw/glfw_window.hpp"

namespace Fge
{

void GraphicManager::create_window()
{
  auto glfw_window = std::make_shared<Glfw::GlfwWindow>();
  glfw_window->create_window();
  glfw_window->init_open_gl();

  window = glfw_window;
}

void GraphicManager::flush() { window->flush(); }

} // namespace Fge
