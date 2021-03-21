#include <date/date.h>
#include <fmt/core.h>

#include "io_log_sink.hpp"
#include "log.hpp"

namespace fge
{

std::string build_log_string(const log_info &i)
{
  std::string type;

  switch (i.type)
  {
  case log_level::debug:
    type = "DEBUG";
    break;
  case log_level::trace:
    type = "TRACE";
    break;
  case log_level::warning:
    type = "\033[1;33mWARNING\033[0m";
    break;
  case log_level::info:
    type = "\033[1;34mINFO\033[0m";
    break;
  case log_level::error:
    type = "\033[1;31mERROR\033[0m";
    break;
  }

  std::ostringstream prefix;
  prefix << i.thread_id;
  using namespace date;
  prefix << " " << i.time_point;
  prefix << " [" << type << "]";

  if (i.tag.size() > 0)
  {
    prefix << " (" << i.tag << ")";
  }

  prefix << " " << i.msg << std::endl;

  return prefix.str();
}

void io_log_sink::process_log_info(const std::vector<log_info> &info)
{
  std::ostringstream log_str_stream;

  for (const auto &i : info)
  {
    log_str_stream << build_log_string(i);
  }

  fmt::print(stderr, "{}", log_str_stream.str());
  std::fflush(stderr);
}

void io_log_sink::process_log_info(const log_info &info)
{
  fmt::print(stderr, "{}", build_log_string(info));
  std::fflush(stderr);
}

} // namespace Fge
