#pragma once

#include <chrono>
#include <mutex>
#include <thread>

#include <fmt/core.h>

#include "log_sink.hpp"
#include "util/assert.hpp"

namespace fge
{

enum class log_level
{
  error   = 0,
  warning = 1,
  info    = 2,
  trace   = 3,
  debug   = 4
};

enum class log_mode
{
  sync,
  async
};

struct log_info
{
  std::chrono::time_point<std::chrono::system_clock> time_point;
  log_level                                          type = log_level::debug;
  std::thread::id                                    thread_id;
  std::string                                        msg;
  std::string                                        tag;
};

extern std::thread          log_thread;
extern std::mutex           log_mutex;
extern std::vector<log_info> log_queue;

extern std::unique_ptr<log_sink> sink;
extern std::mutex               sink_mutex;

extern log_level logging_level;

extern bool stop_logging;

extern log_mode logging_mode;

void termination_handler(int signum);

void watch_log_queue();

void process_log_queue();

template <class T> void set_sink()
{
  sink_mutex.lock();
  sink = std::make_unique<T>();
  sink_mutex.unlock();
}

void terminate_logger();

template <class... Targs>
void error(const std::string &tag,
           const std::string &format_string,
           Targs &&...args)
{
  msg(log_level::error, tag, format_string, std::forward<Targs>(args)...);
}

template <class... Targs>
void warning(const std::string &tag,
             const std::string &format_string,
             Targs &&...args)
{
  msg(log_level::warning, tag, format_string, std::forward<Targs>(args)...);
}

template <class... Targs>
void info(const std::string &tag,
          const std::string &format_string,
          Targs &&...args)
{
  msg(log_level::info, tag, format_string, std::forward<Targs>(args)...);
}

template <class... Targs>
void debug(const std::string &tag,
           const std::string &format_string,
           Targs &&...args)
{
  msg(log_level::debug, tag, format_string, std::forward<Targs>(args)...);
}

template <class... Targs>
void trace(const std::string &tag,
           const std::string &format_string,
           Targs &&...args)
{
  msg(log_level::trace, tag, format_string, std::forward<Targs>(args)...);
}

void set_log_level(log_level type);

template <class TLogSink>
void start_logger([[maybe_unused]] log_level   level         = log_level::debug,
                  [[maybe_unused]] log_mode    mode          = log_mode::sync,
                  [[maybe_unused]] std::size_t reserve_count = 2000)
{
#ifndef FGE_LOG_DISABLE

  stop_logging = false;

  set_log_level(level);
  set_sink<TLogSink>();

  if (mode == log_mode::async)
  {
    log_queue.reserve(reserve_count);
    log_thread = std::thread(watch_log_queue);
  }

  logging_mode = mode;

  std::string log_mode_str;
  if (mode == log_mode::async)
  {
    log_mode_str = "async";
  }
  else if (logging_mode == log_mode::sync)
  {
    log_mode_str = "sync";
  }
  else
  {
    log_mode_str = "unknown";
  }

  trace("Log", "Start logger in {} mode", log_mode_str);
#endif
}

template <class... Targs>
static void msg([[maybe_unused]] log_level          type,
                [[maybe_unused]] const std::string &tag,
                [[maybe_unused]] const std::string &format_string,
                [[maybe_unused]] Targs &&...args)
{
#ifndef FGE_LOG_DISABLE

  if (type > logging_level)
  {
    return;
  }

  const auto time_point = std::chrono::system_clock::now();
  auto       log_msg = fmt::format(format_string, std::forward<Targs>(args)...);

  if (logging_mode == log_mode::sync)
  {
    log_info info{};
    info.time_point = time_point;
    info.type       = type;
    info.thread_id  = std::this_thread::get_id();
    info.msg        = log_msg;
    info.tag        = tag;

    sink->process_log_info(info);
  }
  else
  {
    log_mutex.lock();
    auto &info = log_queue.emplace_back();

    info.time_point = time_point;
    info.type       = type;
    info.thread_id  = std::this_thread::get_id();
    info.msg        = log_msg;
    info.tag        = tag;
    log_mutex.unlock();
  }
#endif
}

} // namespace Fge
