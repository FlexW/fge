#include <gtest/gtest.h>

#include "ecs/fwd.hpp"

using namespace fge;

TEST(EcsRegistry, CreateEntity)
{
  ecs::registry registry;

  const auto entity1 = registry.create();
  const auto entity2 = registry.create();

  EXPECT_EQ(entity1.id(), 0);
  EXPECT_EQ(entity1.version(), 0);
  EXPECT_EQ(entity2.id(), 1);
  EXPECT_EQ(entity1.version(), 0);
}

TEST(EcsRegistry, RecycleEntity)
{
  ecs::registry registry;

  const auto entity = registry.create();
  registry.remove(entity);

  const auto recycled_entity = registry.create();

  EXPECT_EQ(recycled_entity.id(), 0);
  EXPECT_EQ(recycled_entity.version(), 1);
}

TEST(EcsRegistry, CheckEntityValid)
{
  ecs::registry registry;

  const auto entity = registry.create();

  EXPECT_TRUE(registry.valid(entity));

  registry.remove(entity);

  EXPECT_FALSE(registry.valid(entity));
}

TEST(EcsRegistry, EmplaceComponent)
{
  ecs::registry registry;

  const auto entity = registry.create();

  registry.emplace<int>(entity, 20);
  registry.emplace<long>(entity, 212);

  const auto int_component = registry.get<int>(entity);
  const auto long_component = registry.get<long>(entity);

  EXPECT_EQ(int_component, 20);
  EXPECT_EQ(long_component, 212);
}
