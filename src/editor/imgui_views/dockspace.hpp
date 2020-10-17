#pragma once

#include "event/event.hpp"
#include "event/signal.hpp"

namespace Fge::EditorViews
{

struct SignalSaveScene : Event
{
};

struct SignalLoadScene : Event
{
};

class DockSpace
{
public:
  Signal<SignalSaveScene> signal_save_scene;
  Signal<SignalLoadScene> signal_load_scene;

  void draw();
};

} // namespace Fge::EditorViews
