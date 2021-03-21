#pragma once

#include <vector>

namespace fge
{

struct log_info;

class log_sink
{
public:
  virtual ~log_sink();

  virtual void process_log_info(const std::vector<log_info> &info);

  virtual void process_log_info(const log_info &info);
};

} // namespace fge
