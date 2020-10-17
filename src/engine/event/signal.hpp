#pragma once

#include "member_function_handler.hpp"
#include "std.hpp"

namespace Fge
{

template <typename TSignal> class Signal
{
public:
  void publish(const TSignal *const signal)
  {
    for (auto &handler : handlers)
    {
      if (handler != nullptr)
      {
        const auto result = handler->exec(signal);
        if (result)
        {
          break;
        }
      }
    }
  }

  template <typename T>
  void subscribe(T *instance,
                 bool (T::*member_function)(const TSignal *const),
                 int priority = 100)
  {
    uint32_t i = 0;
    for (; i < handlers.size(); ++i)
    {
      if (handlers[i]->get_priority() > priority)
      {
        break;
      }
    }

    if (i >= handlers.size())
    {
      handlers.push_back(
          std::make_shared<MemberFunctionHandler<T, TSignal>>(instance,
                                                              member_function,
                                                              priority));
      return;
    }

    handlers.insert(
        handlers.begin() + i,
        std::make_shared<MemberFunctionHandler<T, TSignal>>(instance,
                                                            member_function,
                                                            priority));
  }

  template <typename T>
  void unsubscribe(T *instance,
                   bool (T::*member_function)(const TSignal *const))
  {
    for (uint32_t i = 0; i < handlers.size(); ++i)
    {
      auto handler =
          static_cast<MemberFunctionHandler<T, TSignal>>(handlers[i]);

      if (handler->get_instance() == instance &&
          handler->get_member_function() == member_function)
      {
        handlers.erase(handlers.begin() + i);
        break;
      }
    }
  }

private:
  std::vector<std::shared_ptr<HandlerFunctionBase>> handlers;
};

} // namespace Fge
