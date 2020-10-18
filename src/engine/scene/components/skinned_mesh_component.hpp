#pragma once

#pragma once

#include "graphic/render_info.hpp"
#include "graphic/skinned_mesh.hpp"
#include "scene/component.hpp"

namespace Fge
{

class SkinnedMeshComponent : public Component
{
public:
  SkinnedMeshComponent(
      Actor *            owner,
      int                update_order = 1000,
      const std::string &type_name    = "Fge::SkinnedMeshComponent");

  ~SkinnedMeshComponent();

  void set_mesh_from_file(const std::string &filepath);

  const std::string &get_mesh_filepath() const { return mesh_filepath; }

  void play_animation(const std::string &name);

  void play_animation_endless(const std::string &name);

  void stop_current_animation();

protected:
  void create() override;

  void update(float delta_time) override;

  void render() override;

private:
  bool component_created = false;

  std::string mesh_filepath;

  std::shared_ptr<SkinnedMesh> mesh{};

  std::vector<std::shared_ptr<RenderInfo>> render_infos{};

  void create_render_infos();

  void load_mesh();

  void register_render_infos();

  void unregister_render_infos();
};

} // namespace Fge
