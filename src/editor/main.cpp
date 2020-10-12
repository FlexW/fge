#include "application.hpp"
#include "editor_layer.hpp"

int main(int argc, char *argv[])
{
  auto app   = Fge::Application::get_instance();
  auto layer = std::make_unique<Fge::EditorLayer>();

  app->push_layer(std::move(layer));

  app->init_application(argc, argv);

  return app->run();
}
