#pragma once

#include "event.hpp"

namespace Fge
{

class HandlerFunctionBase
{
public:
  HandlerFunctionBase(int priority = 100) : priority(priority) {}

  virtual ~HandlerFunctionBase() = default;

  bool exec(const Event *const event) { return call(event); }

  int get_priority() const { return priority; }

private:
  virtual bool call(const Event *const event) = 0;

  int priority;
};

template <class T, class TEvent>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
  typedef bool (T::*MemberFunction)(const TEvent *const);

  MemberFunctionHandler(T *            instance,
                        MemberFunction member_function,
                        int            priority = 100)
      : HandlerFunctionBase(priority),
        instance{instance},
        member_function{member_function} {};

  bool call(const Event *evnt)
  {
    return (instance->*member_function)(static_cast<const TEvent *const>(evnt));
  }

  T *get_instance() const { return instance; }

  MemberFunction get_member_function() const { return member_function; }

private:
  T *instance;

  MemberFunction member_function;
};

} // namespace Fge
