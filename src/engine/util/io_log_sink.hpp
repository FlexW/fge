#pragma once

#include "log_sink.hpp"

namespace fge
{

class io_log_sink : public log_sink
{
public:
  void process_log_info(const std::vector<log_info> &info) override;

  void process_log_info(const log_info &info) override;
};

} // namespace fge
