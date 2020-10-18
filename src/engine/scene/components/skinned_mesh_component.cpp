#include "skinned_mesh_component.hpp"
#include "application.hpp"
#include "graphic/render_info.hpp"
#include "log/log.hpp"
#include "scene/actor.hpp"
#include "util/assert.hpp"

namespace Fge
{

SkinnedMeshComponent::SkinnedMeshComponent(Actor *            owner,
                                           int                update_order,
                                           const std::string &type_name)
    : Component(owner, update_order, type_name)
{
}

SkinnedMeshComponent::~SkinnedMeshComponent() { unregister_render_infos(); }

void SkinnedMeshComponent::set_mesh_from_file(const std::string &filepath)
{
  mesh_filepath = filepath;

  load_mesh();

  if (component_created)
  {
    unregister_render_infos();
    create_render_infos();
    register_render_infos();
  }
}

void SkinnedMeshComponent::play_animation(const std::string &name)
{
  mesh->play_animation(name);
}

void SkinnedMeshComponent::play_animation_endless(const std::string &name)
{
  mesh->play_animation_endless(name);
}

void SkinnedMeshComponent::stop_current_animation()
{
  mesh->stop_current_animation();
}

void SkinnedMeshComponent::create()
{
  component_created = true;
  create_render_infos();
  register_render_infos();

  const auto &world_mat = owner->get_world_transform();
  for (auto sub_mesh : mesh->get_sub_meshes())
  {
    sub_mesh->get_material()->set_uniform("world_mat", world_mat);
  }
}

void SkinnedMeshComponent::update(float /*delta_time*/)
{
  FGE_ASSERT(owner);
  const auto &world_mat = owner->get_world_transform();

  const auto &bone_transforms = mesh->compute_bone_transforms();

  for (auto sub_mesh : mesh->get_sub_meshes())
  {
    sub_mesh->get_material()->set_world_matrix(world_mat);
    sub_mesh->get_material()->set_bone_transforms(bone_transforms);
  }
}

void SkinnedMeshComponent::render() {}

void SkinnedMeshComponent::create_render_infos()
{
  if (!mesh)
  {
    return;
  }

  trace("SkinnedMeshComponent", "Create render infos");

  render_infos.clear();

  for (auto sub_mesh : mesh->get_sub_meshes())
  {
    auto render_info =
        std::make_shared<RenderInfo>(sub_mesh->get_vertex_array(),
                                     sub_mesh->get_index_buffer(),
                                     sub_mesh->get_material());

    render_infos.push_back(render_info);
  }

  trace("SkinnedMeshComponent", "Created {} render infos", render_infos.size());
}

void SkinnedMeshComponent::load_mesh()
{
  if (mesh_filepath == "")
  {
    return;
  }

  trace("SkinnedMeshComponent", "Load mesh");

  auto app         = Application::get_instance();
  auto res_manager = app->get_resource_manager();

  try
  {
    auto mesh  = res_manager->load_skinned_mesh(mesh_filepath);
    this->mesh = mesh;
  }
  catch (std::runtime_error &error)
  {
    warning("SkinnedMeshComponent", "Could not load mesh: {}", error.what());
  }
}

void SkinnedMeshComponent::register_render_infos()
{
  trace("SkinnedMeshComponent", "Register render infos");

  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto renderer        = graphic_manager->get_renderer();

  for (auto render_info : render_infos)
  {
    trace("SkinnedMeshComponent", "Register render info");
    renderer->register_renderable(render_info);
  }
}

void SkinnedMeshComponent::unregister_render_infos()
{
  trace("SkinnedMeshComponent", "Unregister render infos");

  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto renderer        = graphic_manager->get_renderer();

  for (auto render_info : render_infos)
  {
    trace("SkinnedMeshComponent", "Unregister render info");
    renderer->unregister_renderable(render_info);
  }
}

} // namespace Fge
