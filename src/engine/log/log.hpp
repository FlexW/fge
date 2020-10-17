#pragma once

#include <fmt/core.h>

#include "log_sink.hpp"
#include "std.hpp"
#include "util/assert.hpp"

namespace Fge
{

enum class LogType
{
  Error   = 0,
  Warning = 1,
  Info    = 2,
  Trace   = 3,
  Debug   = 4
};

enum class LogMode
{
  Sync,
  Async
};

struct LogInfo
{
  std::chrono::time_point<std::chrono::system_clock> time_point;
  LogType                                            type = LogType::Debug;
  std::thread::id                                    thread_id;
  std::string                                        msg;
  std::string                                        tag;
};

extern std::thread          log_thread;
extern std::mutex           log_mutex;
extern std::vector<LogInfo> log_queue;

extern std::unique_ptr<LogSink> sink;
extern std::mutex               sink_mutex;

extern LogType log_level;

extern bool stop_logging;

extern LogMode logging_mode;

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
           Targs &&... args)
{
  msg(LogType::Error, tag, format_string, std::forward<Targs>(args)...);
}

template <class... Targs>
void warning(const std::string &tag,
             const std::string &format_string,
             Targs &&... args)
{
  msg(LogType::Warning, tag, format_string, std::forward<Targs>(args)...);
}

template <class... Targs>
void info(const std::string &tag,
          const std::string &format_string,
          Targs &&... args)
{
  msg(LogType::Info, tag, format_string, std::forward<Targs>(args)...);
}

template <class... Targs>
void debug(const std::string &tag,
           const std::string &format_string,
           Targs &&... args)
{
  msg(LogType::Debug, tag, format_string, std::forward<Targs>(args)...);
}

template <class... Targs>
void trace(const std::string &tag,
           const std::string &format_string,
           Targs &&... args)
{
  msg(LogType::Trace, tag, format_string, std::forward<Targs>(args)...);
}

void set_log_level(LogType type);

template <class TLogSink>
void start_logger(LogType     log_level,
                  LogMode     log_mode,
                  std::size_t reserve_count = 2000)
{
#ifndef FGE_LOG_DISABLE

  stop_logging = false;

  set_log_level(log_level);
  set_sink<TLogSink>();

  if (log_mode == LogMode::Async)
  {
    log_queue.reserve(reserve_count);
    log_thread = std::thread(watch_log_queue);
  }

  logging_mode = log_mode;

  std::string log_mode_str;
  if (log_mode == LogMode::Async)
  {
    log_mode_str = "async";
  }
  else if (log_mode == LogMode::Sync)
  {
    log_mode_str = "sync";
  }
  else
  {
    log_mode_str = "unknown";
  }

  trace("Log", "Start logging system in {} mode", log_mode_str);

#endif
}

template <class... Targs>
static void msg(LogType            type,
                const std::string &tag,
                const std::string &format_string,
                Targs &&... args)
{
#ifndef FGE_LOG_DISABLE

  if (type > log_level)
  {
    return;
  }

  const auto time_point = std::chrono::system_clock::now();
  auto       log_msg = fmt::format(format_string, std::forward<Targs>(args)...);

  if (logging_mode == LogMode::Sync)
  {
    LogInfo log_info;
    log_info.time_point = time_point;
    log_info.type       = type;
    log_info.thread_id  = std::this_thread::get_id();
    log_info.msg        = log_msg;
    log_info.tag        = tag;

    sink->process_log_info(log_info);
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
