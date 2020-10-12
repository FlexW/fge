#pragma once

#include "member_function_handler.hpp"
#include "std.hpp"

namespace Fge
{

using HandlerList = std::vector<std::shared_ptr<HandlerFunctionBase>>;

class EventManager
{
public:
  ~EventManager()
  {
    // clear();
  }

  template <typename TEvent> void publish(const TEvent *const event)
  {
    auto handlers = subscribers[typeid(TEvent)];

    if (handlers == nullptr)
    {
      return;
    }

    for (auto &handler : *handlers)
    {
      if (handler != nullptr)
      {
        const auto result = handler->exec(event);
        if (result)
        {
          break;
        }
      }
    }
  }

  template <class T, class TEvent>
  void subscribe(T *instance,
                 bool (T::*member_function)(const TEvent *const),
                 int priority = 100)
  {
    auto handlers = subscribers[typeid(TEvent)];

    // First time initialization
    if (handlers == nullptr)
    {
      handlers                    = new HandlerList();
      subscribers[typeid(TEvent)] = handlers;
    }

    unsigned i = 0;
    for (; i < handlers->size(); ++i)
    {
      if ((*handlers)[i]->get_priority() > priority)
      {
        break;
      }
    }

    if (i >= handlers->size())
    {
      handlers->push_back(
          std::make_shared<MemberFunctionHandler<T, TEvent>>(instance,
                                                             member_function,
                                                             priority));
      return;
    }

    handlers->insert(
        handlers->begin() + i,
        std::make_shared<MemberFunctionHandler<T, TEvent>>(instance,
                                                           member_function,
                                                           priority));
  }

  template <class T, class TEvent>
  void unsubscribe(T *instance, bool (T::*member_function)(const TEvent *const))
  {
    auto handlers = subscribers[typeid(TEvent)];

    if (handlers == nullptr)
    {
      return;
    }

    for (unsigned i = 0; i < handlers->size(); ++i)
    {
      auto handler = static_cast<MemberFunctionHandler<T, TEvent>>(handlers[i]);

      if (handler->get_instance() == instance &&
          handler->get_member_function() == member_function)
      {
        handlers->erase(handlers->begin() + i);
        break;
      }
    }
  }

  // void clear();

private:
  std::unordered_map<std::type_index, HandlerList *> subscribers;
};

} // namespace Fge
