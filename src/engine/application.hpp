#pragma once

#include <memory>

#include "broadcast.hpp"
#include "gfx/renderer.hpp"

namespace fge
{

/**
 * @brief Game Engine
 *
 * Main application class.
 */
class application
{
public:
  /**
   * @brief Create a new application object.
   *
   * Don't forget to call init() after the creation.
   */
  application();

  /**
   * @brief Initializes the application.
   *
   * @param argc Argument count.
   * @param argv Arguments.
   */
  void init(int argc, char *argv[]);

  /**
   * @brief Start the applications main loop.
   *
   * This function will only return if the application exited.
   *
   * @return Status code.
   */
  int run();

private:
  std::unique_ptr<broadcast>     application_broadcast;
  std::unique_ptr<gfx::renderer> renderer;

  bool close_application = false;

  void main_loop();

  void terminate();

  void init_application();
};

} // namespace fge
