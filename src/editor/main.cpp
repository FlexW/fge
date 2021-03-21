#include "application.hpp"

int main(int argc, char *argv[])
{
  fge::application app;
  app.init(argc, argv);
  return app.run();
}
