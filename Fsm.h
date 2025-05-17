// A generic FSM engine that real FSMs inherit from.
#pragma once

#include <map>
#include <utility>

template <typename Derived>

class Fsm
{
  public:
    using Event = int;
    using State = int;
    using Action = int;

    void inject_event(Event event, void *context = nullptr)
    {
        // loop until NULL_EVENT
        while (event != Derived::EV_NULL_EVENT)
        {
            // How this is handled is up to the derived class
            static_cast<Derived *>(this)->log_event(event);

            auto [action, next_state] = Derived::transitions[_state][event];

            if (action == Derived::AC_IGNORE_EVENT)
            {
                // Do not change state
                event = Derived::EV_NULL_EVENT;
            }
            else
            {
                // Change state
                event = static_cast<Derived *>(this)->handle_action(action, context);

                if (_state != next_state)
                {
                    // How this is handled is up to the derived class
                    static_cast<Derived *>(this)->log_state(_state, next_state);

                    _state = next_state;
                }
            }
        }
    }

    State get_state() const
    {
        return _state;
    }

  protected:
    void set_initial(State s)
    {
        _state = s;
    }

  private:
    State _state;
};
