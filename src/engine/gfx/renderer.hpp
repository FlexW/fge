#pragma once

#include <memory>

#include "broadcast.hpp"
#include "window.hpp"

namespace fge::gfx
{

/**
 * @brief The engines renderer.
 *
 * Other subsystems of the engine should interact with this class if
 * they want to do anything render related.
 */
class renderer
{
public:
  /**
   * @brief Create a new renderer.
   */
  renderer(broadcast *application_broadcast);

  /**
   * @brief Initialize the renderer.
   *
   * This function needs to be called before any other function on the renderer.
   */
  void init();

  /**
   * @brief Notifies the renderer that it can start to render the
   * current frame.
   */
  void render_frame();

  window *get_renderer_window() const;

  void terminate();

private:
  broadcast *application_broadcast;

  std::unique_ptr<window> renderer_window = nullptr;
};

} // namespace fge::gfx
