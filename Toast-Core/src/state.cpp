#include "toast/state.hpp"
#include "toast/memory.hpp"
#include "toast/util.hpp"

using namespace Toast;
using namespace std;

static vector<void (*)(State, State)> __trackers;
static vector<void (*)(State)> __tickers;
static vector<IterativeBase*> __iterative;

static State _DISABLED(STATE_DISABLED);
static State _AUTO(STATE_AUTO);
static State _TELEOP(STATE_TELEOP);
static State _TEST(STATE_TEST);

static int _last_ordinal_state = 0;

void States::Internal::set_state(int id) {
    int last = Memory::Shared::get()[ADDR_STATE_CURRENT];
    Memory::Shared::get()[ADDR_STATE_CURRENT] = id;
    Memory::Shared::get()[ADDR_STATE_LAST] = last;
}

void States::Internal::set_tick_timing(int ms) {
    Memory::Shared::get()[ADDR_TICK_TIMING] = ms;
}

int States::Internal::get_tick_timing() {
    return Memory::Shared::get()[ADDR_TICK_TIMING];
}

State States::DISABLED() {
    return _DISABLED;
}

State States::AUTO() {
    return _AUTO;
}

State States::TELEOP() {
    return _TELEOP;
}

State States::TEST() {
    return _TEST;
}

State States::from_ordinal(int id) {
    if (id == STATE_DISABLED)   return _DISABLED;
    if (id == STATE_AUTO)       return _AUTO;
    if (id == STATE_TELEOP)     return _TELEOP;
    if (id == STATE_TEST)       return _TEST;
    
    return _DISABLED;
}

int States::current_state_ordinal() {
    return Memory::Shared::get()[ADDR_STATE_CURRENT];
}

int States::last_state_ordinal() {
    return Memory::Shared::get()[ADDR_STATE_LAST];
}

State States::current_state() {
    return States::from_ordinal(States::current_state_ordinal());
}

State States::last_state() {
    return States::from_ordinal(States::last_state_ordinal());
}

void States::start_tracker() {
    while (is_process_alive(Memory::Shared::get_bootstrap_pid())) {
        int current_state = States::current_state_ordinal();
        State _cur = States::from_ordinal(current_state);
        
        if (current_state != _last_ordinal_state) {
            // Transition Occured
            State _old = States::from_ordinal(_last_ordinal_state);
            for (auto func : __trackers) {
                func(_old, _cur);
            }
            for (auto it : __iterative) {
                if (current_state == STATE_DISABLED)
                    it->disabledInit();
                else if (current_state == STATE_AUTO)
                    it->autonomousInit();
                else if (current_state == STATE_TELEOP)
                    it->teleopInit();
                else if (current_state == STATE_TEST)
                    it->testInit();
            }
        }
        
        // Periodic
        for (auto func : __tickers) {
            func(_cur);
        }
        for (auto it : __iterative) {
            if (current_state == STATE_DISABLED)
                it->disabledPeriodic();
            else if (current_state == STATE_AUTO)
                it->autonomousPeriodic();
            else if (current_state == STATE_TELEOP)
                it->teleopPeriodic();
            else if (current_state == STATE_TEST)
                it->testPeriodic();
        }
        
        sleep_ms(States::Internal::get_tick_timing());
        _last_ordinal_state = current_state;
    }
}

void States::register_tracker(void (*func)(State, State)) {
    __trackers.push_back(func);
}

void States::register_ticker(void (*func)(State)) {
    __tickers.push_back(func);
}

void States::register_iterative(IterativeBase *iterative) {
    __iterative.push_back(iterative);
}