#pragma once

namespace fge
{

class application;

class layer
{
public:
  layer(application *app);

  virtual void init() = 0;

  virtual void update(float delta_time) = 0;

  virtual void terminate() = 0;

protected:
  application *app;
};

} // namespace fge
