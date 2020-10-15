#pragma once

#include "std.hpp"

namespace Fge
{

struct LogInfo;

class LogSink
{
public:
  virtual ~LogSink() {}

  virtual void process_log_info(const std::vector<LogInfo> & /*info*/) {}

  virtual void process_log_info(const LogInfo & /*info*/) {}
};

} // namespace Fge
