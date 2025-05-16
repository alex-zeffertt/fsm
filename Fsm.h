// A generic FSM engine that real FSMs inherit from.
#pragma once

#include <map>
#include <utility>

template <typename Table>

class Fsm
{
  public:
    using Event = int;
    using State = int;
    using Action = int;

    void inject_event(Event event, void *context = nullptr)
    {
        // loop until NULL_EVENT
        while (event != Table::EV_NULL_EVENT)
        {
            // How this is handled is up to the derived class
            log_event(event);

            auto [action, next_state] = Table::transitions[_state][event];

            if (action == Table::AC_IGNORE_EVENT)
            {
                // Do not change state
                event = Table::EV_NULL_EVENT;
            }
            else
            {
                // Change state
                event = handle_action(action, context);

                if (_state != next_state)
                {
                    // How this is handled is up to the derived class
                    log_state(_state, next_state);

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
    virtual Event handle_action(Action action, void *context) = 0;
    virtual void log_event(Event event) const {};
    virtual void log_state(State oldstate, State newstate) const {};

    void set_initial(State s)
    {
        _state = s;
    }

  private:
    State _state;
};
