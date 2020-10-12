#include <date/date.h>
#include <fmt/core.h>

#include "io_log_sink.hpp"
#include "log.hpp"
#include "std.hpp"

namespace Fge
{

void IOLogSink::process_log_info(const std::vector<LogInfo> &info)
{
  std::ostringstream log_str_stream;

  for (const auto &i : info)
  {
    std::string type;

    switch (i.type)
    {
    case LogType::Debug:
      type = "DEBUG";
      break;
    case LogType::Trace:
      type = "TRACE";
      break;
    case LogType::Warning:
      type = "\033[1;33mWARNING\033[0m";
      break;
    case LogType::Info:
      type = "\033[1;34mINFO\033[0m";
      break;
    case LogType::Error:
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

    log_str_stream << prefix.str();
  }

  fmt::print(stderr, "{}", log_str_stream.str());
  std::fflush(stderr);
}

} // namespace Fge
