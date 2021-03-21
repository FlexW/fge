#include "log_sink.hpp"

namespace fge
{

log_sink::~log_sink() = default;

void log_sink::process_log_info(const std::vector<log_info> & /*info*/) {}

void log_sink::process_log_info(const log_info & /*info*/) {}

} // namespace fge
