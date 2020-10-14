#include "mesh_component.hpp"
#include "application.hpp"
#include "graphic/render_info.hpp"
#include "log/log.hpp"
#include "scene/actor.hpp"
#include "util/assert.hpp"

namespace Fge
{

MeshComponent::MeshComponent(Actor *            owner,
                             int                update_order,
                             const std::string &type_name)
    : Component(owner, update_order, type_name)
{
}

MeshComponent::~MeshComponent() { unregister_render_infos(); }

void MeshComponent::set_mesh_from_file(const std::string &filepath)
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

void MeshComponent::create()
{
  component_created = true;
  create_render_infos();
  register_render_infos();
}

void MeshComponent::update(float /*delta_time*/)
{
  FGE_ASSERT(owner);
  const auto &world_mat = owner->get_world_transform();

  for (auto sub_mesh : mesh->get_sub_meshes())
  {
    sub_mesh->get_material()->set_uniform("world_mat", world_mat);
  }
}

void MeshComponent::render() {}

void MeshComponent::create_render_infos()
{
  if (!mesh)
  {
    return;
  }

  trace("MeshComponent", "Create render infos");

  render_infos.clear();

  for (auto sub_mesh : mesh->get_sub_meshes())
  {
    auto render_info =
        std::make_shared<RenderInfo>(sub_mesh->get_vertex_array(),
                                     sub_mesh->get_index_buffer(),
                                     sub_mesh->get_material());

    render_infos.push_back(render_info);
  }

  trace("MeshComponent", "Created {} render infos", render_infos.size());
}

void MeshComponent::load_mesh()
{
  if (mesh_filepath == "")
  {
    return;
  }

  trace("MeshComponent", "Load mesh");

  auto app         = Application::get_instance();
  auto res_manager = app->get_resource_manager();

  try
  {
    auto mesh  = res_manager->load_mesh(mesh_filepath);
    this->mesh = mesh;
  }
  catch (std::runtime_error &error)
  {
    warning("MeshComponent", "Could not load mesh: {}", error.what());
  }
}

void MeshComponent::register_render_infos()
{
  trace("MeshComponent", "Register render infos");

  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto renderer        = graphic_manager->get_renderer();

  for (auto render_info : render_infos)
  {
    trace("MeshComponent", "Register render info");
    renderer->register_renderable(render_info);
  }
}

void MeshComponent::unregister_render_infos()
{
  trace("MeshComponent", "Unregister render infos");

  auto app             = Application::get_instance();
  auto graphic_manager = app->get_graphic_manager();
  auto renderer        = graphic_manager->get_renderer();

  for (auto render_info : render_infos)
  {
    trace("MeshComponent", "Unregister render info");
    renderer->unregister_renderable(render_info);
  }
}

} // namespace Fge
