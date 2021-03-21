#include "log.hpp"

namespace fge
{

std::thread              log_thread;
std::mutex               log_mutex;
std::vector<log_info>    log_queue;
std::mutex               sink_mutex;
std::unique_ptr<log_sink> sink;
log_mode                  logging_mode;

bool stop_logging = false;

log_level logging_level = log_level::debug;

void terminate_logger()
{
#ifndef FGE_LOG_DISABLE

  trace("Log", "Stop logger");
  stop_logging = true;

  if (logging_mode == log_mode::async)
  {
    log_thread.join();
  }

#endif
}

void set_log_level(log_level type) { logging_level = type; }

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

  const std::vector<log_info> log_queue_copy(std::move(log_queue));
  log_queue.clear();

  log_mutex.unlock();

  sink_mutex.lock();
  if (sink)
  {
    sink->process_log_info(log_queue_copy);
  }
  sink_mutex.unlock();
}

} // namespace fge
