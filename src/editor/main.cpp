#include "application.hpp"
#include "editor_layer.hpp"

int main(int argc, char *argv[])
{
  fge::application app;

  app.add_application_layer<fge::editor_layer>();

  app.init(argc, argv);
  return app.run();
}
