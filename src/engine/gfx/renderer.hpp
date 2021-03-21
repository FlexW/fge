#pragma once

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
};

} // namespace fge::gfx
