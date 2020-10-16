#include <sys/time.h>

#include "std.hpp"
#include "time.hpp"

namespace Fge
{

int64_t get_current_time_millis()
{
  timeval t;
  gettimeofday(&t, nullptr);

  auto ret = t.tv_sec * 1000 + t.tv_usec / 1000;

  return ret;
}

} // namespace Fge
