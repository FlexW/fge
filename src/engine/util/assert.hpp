#pragma once

#include "std.hpp"

#define FGE_DO_ASSERT_MSG(file, line, expr, msg)                               \
  do                                                                           \
  {                                                                            \
    if (!(expr))                                                               \
    {                                                                          \
      std::cerr << "Assertion '" #expr "' failed in " << (file) << " on line " \
                << (line) << ": " << (msg) << std::endl;                       \
      std::exit(EXIT_FAILURE);                                                 \
    }                                                                          \
  } while (false)

#define FGE_ASSERT_MSG(expr, msg)                                              \
  FGE_DO_ASSERT_MSG(__FILE__, __LINE__, expr, msg)

#define FGE_DO_ASSERT(file, line, expr)                                        \
  do                                                                           \
  {                                                                            \
    if (!(expr))                                                               \
    {                                                                          \
      std::cerr << "Assertion '" #expr "' failed in " << (file) << " on line " \
                << (line) << std::endl;                                        \
      std::exit(EXIT_FAILURE);                                                 \
    }                                                                          \
  } while (false)

#define FGE_ASSERT(expr) FGE_DO_ASSERT(__FILE__, __LINE__, expr)

#define FGE_DO_FAIL(file, line, msg)                                           \
  do                                                                           \
  {                                                                            \
    std::cerr << "Failed in " << (file) << " on line " << (line) << ": "       \
              << (msg) << std::endl;                                           \
    std::exit(EXIT_FAILURE);                                                   \
  } while (false)

#define FGE_FAIL(msg) FGE_DO_FAIL(__FILE__, __LINE__, msg)
