#pragma once

#include "graphic/mesh.hpp"
#include "graphic/render_info.hpp"
#include "scene/component.hpp"

namespace Fge
{

class MeshComponent : public Component
{
public:
  MeshComponent(Actor *            owner,
                int                update_order = 100,
                const std::string &type_name    = "Fge::MeshComponent");

  ~MeshComponent();

  void set_mesh_from_file(const std::string &filepath);

  const std::string &get_mesh_filepath() const { return mesh_filepath; }

protected:
  void create() override;

  void update(float delta_time) override;

  void render() override;

private:
  bool component_created = false;

  std::string mesh_filepath;

  std::shared_ptr<Mesh> mesh{};

  std::vector<std::shared_ptr<RenderInfo>> render_infos{};

  void create_render_infos();

  void load_mesh();

  void register_render_infos();

  void unregister_render_infos();
};

} // namespace Fge
