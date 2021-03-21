#include <gtest/gtest.h>

#include "broadcast.hpp"

struct test_event
{
  int value;
};

struct different_test_event
{
  int value;
};

TEST(Broadcast, PublishEvent) {
  fge::broadcast broadcast;

  bool test_event_handler_called1 = false;
  broadcast.subscribe<test_event>([&](const test_event &event) {
    test_event_handler_called1 = true;
    EXPECT_EQ(event.value, 2);
    return false;
  });

  bool test_event_handler_called2 = false;
  broadcast.subscribe<test_event>([&](const test_event &event) {
    test_event_handler_called2 = true;
    EXPECT_EQ(event.value, 2);
    return false;
  });

  test_event event;
  event.value = 2;
  broadcast.publish(event);

  EXPECT_EQ(test_event_handler_called1, true);
  EXPECT_EQ(test_event_handler_called2, true);
}

TEST(
    Broadcast,
    Publish_MultipleHandlersForDifferentEventsSubscribed_CallOnlyRelevantHandlers)
{
  fge::broadcast broadcast;

  bool test_event_handler_called = false;
  broadcast.subscribe<test_event>([&](const test_event &event) {
    test_event_handler_called = true;
    return false;
  });

  bool different_test_event_handler_called = false;
  broadcast.subscribe<different_test_event>(
      [&](const different_test_event &event) {
        different_test_event_handler_called = true;
        return false;
      });

  test_event event;
  event.value = 2;
  broadcast.publish(event);

  EXPECT_TRUE(test_event_handler_called);
  EXPECT_FALSE(different_test_event_handler_called);
}
