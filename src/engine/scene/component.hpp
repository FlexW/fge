#pragma once

#include "std.hpp"

namespace Fge
{

class Actor;

class Component
{
public:
  const std::string type_name;

  Component(Actor *            owner,
            int                update_order = 100,
            const std::string &type_name    = "Fge::Component");

  virtual ~Component() = default;

  void on_create();

  void on_fixed_update(float frametime);

  void on_update(float delta_time);

  void on_render();

  int get_update_order() const { return update_order; }

protected:
  Actor *owner{};

  int update_order{};

  virtual void create();

  virtual void fixed_update(float delta_time);

  virtual void update(float delta_time);

  virtual void render();
};

} // namespace Fge
