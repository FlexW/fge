#pragma once

namespace fge
{

class delta_time
{
public:
  void start();

  void update();

  float get() const;

private:
  float last_frame;
  float current_delta_time;
};

} // namespace fge
