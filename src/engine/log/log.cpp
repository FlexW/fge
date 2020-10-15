#include "log.hpp"

namespace Fge
{

std::thread              log_thread;
std::mutex               log_mutex;
std::vector<LogInfo>     log_queue;
std::mutex               sink_mutex;
std::unique_ptr<LogSink> sink;
LogMode                  logging_mode;

bool    stop_logging = false;
LogType log_level    = LogType::Debug;

void terminate_logger()
{
#ifndef FGE_LOG_DISABLE

  trace("Log", "Stop logging system");
  stop_logging = true;
  log_thread.join();

#endif
}

void set_log_level(LogType type) { log_level = type; }

std::string signal_to_string(int signum)
{
  switch (signum)
  {
  case SIGSEGV:
    return "SIGSEGV";
  case SIGTERM:
    return "SIGTERM";
  case SIGABRT:
    return "SIGABRT";
  case SIGINT:
    return "SIGINT";
  case SIGHUP:
    return "SIGHUP";
  case SIGFPE:
    return "SIGFPE";
  default:
    FGE_FAIL("No such signal");
  }
}

void termination_handler(int signum)
{
  info("", "Received signal " + signal_to_string(signum));
  terminate_logger();
  std::exit(1);
}

void watch_log_queue()
{
  using namespace std::chrono_literals;

  while (!stop_logging || log_queue.size() > 20000)
  {
    process_log_queue();

    std::this_thread::sleep_for(1s);
  }
  // Empty queue
  process_log_queue();
}

void process_log_queue()
{
  if (log_queue.size() == 0)
  {
    return;
  }

  log_mutex.lock();

  const std::vector<LogInfo> log_queue_copy(std::move(log_queue));
  log_queue.clear();

  log_mutex.unlock();

  sink_mutex.lock();
  if (sink)
  {
    sink->process_log_info(log_queue_copy);
  }
  sink_mutex.unlock();
}

} // namespace Fge
