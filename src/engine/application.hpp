#pragma once

#include <memory>
#include <vector>

#include "broadcast.hpp"
#include "gfx/renderer.hpp"
#include "layer_stack.hpp"
#include "scn/scene.hpp"
#include "util/time.hpp"

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

  template <typename TLayer> void add_application_layer()
  {
    application_layers.add_layer<TLayer>(this);
  }

  gfx::renderer *get_renderer();

  broadcast *get_application_broadcast() const;

  void close();

private:
  std::unique_ptr<broadcast>     application_broadcast;
  std::unique_ptr<gfx::renderer> renderer;

  bool close_application = false;

  layer_stack application_layers;
  delta_time  delta_timer;

  void main_loop();

  void terminate();

  void init_application();
};

} // namespace fge
