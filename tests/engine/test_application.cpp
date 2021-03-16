#include <string>

#include <gtest/gtest.h>

const char *actualValTrue  = "hello gtest";
const char *actualValFalse = "hello world";
const char *expectVal      = "hello gtest";

TEST(StrCompare, CStrEqual) { EXPECT_STREQ(expectVal, actualValTrue); }

// TEST(StrCompare, CStrNotEqual) { EXPECT_STREQ(expectVal, actualValFalse); }
