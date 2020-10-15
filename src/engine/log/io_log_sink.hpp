#pragma once

#include "log_sink.hpp"

namespace Fge
{

class IOLogSink : public LogSink
{
public:
  void process_log_info(const std::vector<LogInfo> &info) override;

  void process_log_info(const LogInfo &info) override;
};

} // namespace Fge
