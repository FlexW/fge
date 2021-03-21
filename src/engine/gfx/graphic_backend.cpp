#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "backend/backends.hpp"
#include "graphic_backend.hpp"
#include "graphic_backend_context.hpp"
#include "util/assert.hpp"

namespace fge::gfx::graphic_backend
{

static std::unordered_map<std::string,
                          std::function<graphic_backend_context(void)>>
    registered_graphic_backends;

static graphic_backend_context context;

void init() {
  const std::string backend_to_load_name("opengl");

  auto graphic_backends_iter = graphic_backends.find(backend_to_load_name);
  if (graphic_backends_iter == registered_graphic_backends.end())
  {
    throw std::runtime_error("Can not load graphic backend: " +
                             backend_to_load_name);
  }

  context = graphic_backends_iter->second();
}

std::unique_ptr<window> create_window(broadcast *application_broadcast,
                                      const window_create_info &create_info)
{
  FGE_ASSERT(context.create_window);
  return context.create_window(application_broadcast, create_info);
}

} // namespace fge::gfx::graphic_backend
