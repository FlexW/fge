#pragma once

#include <memory>

#include "broadcast.hpp"
#include "gfx/command_bucket.hpp"
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

  /**
   * @brief Returns the window that the renderer uses to draw it's content.
   *
   * @return Window
   */
  window *get_renderer_window() const;

  /**
   * @brief Terminate the renderer.
   *
   * Deallocates all resources. After a call to this method, the renderer needs
   * to be reinitalized with init().
   */
  void terminate();

  /**
   * @brief Create a new command bucket.
   *
   * If you want to draw something on the screen you should use a command bucket
   * for that.
   *
   * @return Command bucket
   */
  command_bucket *create_command_bucket();

  void start_frame();

private:
  broadcast *application_broadcast;

  std::unique_ptr<window> renderer_window = nullptr;
};

} // namespace fge::gfx
