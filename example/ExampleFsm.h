#pragma once

#include "Fsm.h"
#include "ExampleFsmTable.h"
#include <cstdio>

class ExampleFsm : public ExampleFsmTable, public Fsm<ExampleFsmTable>
{
  public:
    ExampleFsm() : closed(false)
    {
        set_initial(ST_CIRCUIT_OPEN);
    }

  private:
    Event handle_action(Action action, void *context) override
    {
        auto func = _handlers[action];
        return func ? ((this)->*func)(context) : EV_NULL_EVENT;
    }

    // action handlers

    Event close_circuit_and_start_timer(void *context)
    {
        closed = true;
        return EV_NULL_EVENT;
    }

    Event open_circuit(void *context)
    {
        closed = false;
        return EV_NULL_EVENT;
    }

    Event emit_button_press(void *context)
    {
        return EV_BUTTON_PRESS;
    }

    // loggers

    void log_event(Event event) const override
    {
        printf("Event: %s\n", event_names[event]);
    }

    void log_state(State oldstate, State newstate) const override
    {
        printf("State change: %s -> %s\n", state_names[oldstate], state_names[newstate]);
    }

    // Private data

    bool closed;
    using Function = Event (ExampleFsm::*)(void *);
    static constexpr const Function _handlers[NUM_ACTIONS] = {
        [AC_IGNORE_EVENT] = nullptr,
        [AC_CLOSE_CIRCUIT_AND_START_TIMER] = &ExampleFsm::close_circuit_and_start_timer,
        [AC_OPEN_CIRCUIT] = &ExampleFsm::open_circuit,
        [AC_EMIT_BUTTON_PRESS] = &ExampleFsm::emit_button_press,
    };
};
