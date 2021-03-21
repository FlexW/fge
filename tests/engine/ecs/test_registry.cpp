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
  registry.destroy(entity);

  const auto recycled_entity = registry.create();

  EXPECT_EQ(recycled_entity.id(), 0);
  EXPECT_EQ(recycled_entity.version(), 1);
}

TEST(EcsRegistry, CheckEntityValid)
{
  ecs::registry registry;

  const auto entity = registry.create();

  EXPECT_TRUE(registry.valid(entity));

  registry.destroy(entity);

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

TEST(EcsRegistry, TryGet_ComponentDoesNotExist_ReturnNothing)
{
  ecs::registry registry;

  const auto entity = registry.create();

  const auto int_component = registry.try_get<int>(entity);

  EXPECT_FALSE(int_component.has_value());
}

TEST(EcsRegistry, TryGet_ComponentDoesExist_ReturnComponent)
{
  ecs::registry registry;

  const auto entity = registry.create();
  registry.emplace<int>(entity, 20);

  const auto int_component = registry.try_get<int>(entity);

  EXPECT_TRUE(int_component.has_value());
  EXPECT_EQ(*int_component.value(), 20);
}

TEST(EcsRegistry, Remove_ComponentExist_RemoveComponent)
{
  ecs::registry registry;

  const auto entity = registry.create();
  registry.emplace<int>(entity, 20);

  registry.remove<int>(entity);

  const auto int_component = registry.try_get<int>(entity);

  EXPECT_FALSE(int_component.has_value());
}

TEST(EcsRegistry, Remove_ThreeComponentExist_RemoveMiddleComponent)
{
  ecs::registry registry;

  const auto entity = registry.create();
  registry.emplace<int>(entity, 20);
  registry.emplace<long>(entity, 30);
  registry.emplace<long long>(entity, 40);

  registry.remove<long>(entity);

  const auto int_component         = registry.get<int>(entity);
  const auto long_component = registry.try_get<long>(entity);
  const auto long_long_component   = registry.get<long long>(entity);

  EXPECT_FALSE(long_component.has_value());
  EXPECT_EQ(int_component, 20);
  EXPECT_EQ(long_long_component, 40);
}

struct comp_component
{
  int a;
  int b;
};

TEST(EcsRegistry, Emplace_ComponentCreated_CanModifyComponent)
{
  ecs::registry registry;

  const auto entity        = registry.create();
  auto &     component     = registry.emplace<comp_component>(entity, 1, 2);

  component.a = 5;
  component.b = 6;

  auto component_from_get = registry.get<comp_component>(entity);

  EXPECT_EQ(component_from_get.a, 5);
  EXPECT_EQ(component_from_get.b, 6);
}
