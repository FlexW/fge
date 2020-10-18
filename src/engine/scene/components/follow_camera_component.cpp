#include "follow_camera_component.hpp"
#include "application.hpp"
#include "graphic/camera_controller.hpp"
#include "math/math.hpp"
#include "scene/actor.hpp"
#include "scene/component.hpp"

namespace Fge
{

FollowCameraComponent::FollowCameraComponent(Actor *            owner,
                                             int                update_order,
                                             const std::string &type_name)
    : Component(owner, update_order, type_name)
{
}

void FollowCameraComponent::update(float /*delta_time*/)
{
  const auto camera_pos = owner->get_position() -
                          owner->get_forward() * horz_dist + UP_VEC * vert_dist;

  const auto target_pos =
      owner->get_position() + owner->get_forward() * target_dist;

  const auto view_mat = glm::lookAt(camera_pos, target_pos, UP_VEC);

  auto &camera_controller = Application::get_instance()
                                ->get_graphic_manager()
                                ->get_camera_controller();

  CameraInfo camera_info{};
  camera_info.position = camera_pos;
  camera_info.view_mat = view_mat;

  camera_controller.set_camera_info(camera_info);
}

} // namespace Fge
