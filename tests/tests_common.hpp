#pragma once

#include <functional>
#include <gtest/gtest.h>
#include <string>

namespace Fge::Tests
{

template <typename TException>
void assert_exception(
    std::function<void()> expression,
    const std::string &   error_message = "No exception thrown!",
    std::function<void(const TException &)> process_exception =
        [](const TException &) {})
{
  try
  {
    expression();
    FAIL() << error_message;
  }
  catch (TException &exception)
  {
    process_exception(exception);
  }
}

} // namespace Fge::Tests
