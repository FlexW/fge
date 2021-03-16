#include <gtest/gtest.h>

#include "ecs/type_info.hpp"

using namespace fge::ecs;

TEST(EcsTypeInfo, Type)
{
  const auto int_id   = sequential_type_info::type<int>();
  const auto float_id = sequential_type_info::type<float>();

  EXPECT_EQ(int_id, 0);
  EXPECT_EQ(float_id, 1);
}
