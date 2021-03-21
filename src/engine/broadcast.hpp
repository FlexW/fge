#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace fge
{

class broadcast_handler_base
{
};

template <typename TEvent>
class broadcast_handler : public broadcast_handler_base
{
public:
  broadcast_handler(const std::function<bool(const TEvent &)> &handler)
      : handler(handler)
  {
  }

  bool call(const TEvent &event) { return handler(event); }

private:
  std::function<bool(const TEvent &)> handler;
};

class broadcast
{
public:
  template <typename TEvent>
  void subscribe(const std::function<bool(const TEvent &)> &handler)
  {
    auto event_handler_lists_iter = event_handler_lists.find(typeid(TEvent));

    if (event_handler_lists_iter == event_handler_lists.end())
    {
      std::vector<std::unique_ptr<broadcast_handler_base>> handler_list;
      event_handler_lists[typeid(TEvent)] = std::move(handler_list);
    }

    event_handler_lists[typeid(TEvent)].emplace_back(
        new broadcast_handler(handler));
  }

  template <typename TEvent> void publish(const TEvent &event)
  {

    auto event_handler_lists_iter = event_handler_lists.find(typeid(TEvent));

    if (event_handler_lists_iter == event_handler_lists.end())
    {
      return;
    }

    for (auto &handler : event_handler_lists_iter->second)
    {
      if (static_cast<broadcast_handler<TEvent> *>(handler.get())->call(event))
      {
        break;
      }
    }
  }

private:
  std::unordered_map<std::type_index,
                     std::vector<std::unique_ptr<broadcast_handler_base>>>
      event_handler_lists;
};

} // namespace fge
