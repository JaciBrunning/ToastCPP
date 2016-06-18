#pragma once

#include "toast/library.hpp"
#include <string>
#include <vector>

#define STATE_DISABLED 0
#define STATE_AUTO 1
#define STATE_TELEOP 2
#define STATE_TEST 3

#define STATE_CURRENT_IDX 0x08
#define STATE_LAST_IDX 0x09
#define STATE_TICK_TIMING_IDX 0x0A

namespace Toast {
    class State { 
        public:
            State(int id) {
                _id = id; // 0 = DISABLED, 1 = AUTO, 2 = TELEOP, 3 = TEST
            }
            
            // Is this state controlled by an operator? This is true for TELEOP and TEST
            // States.
            API bool is_operator() {
                return _id == 2 || _id == 3;
            }
            
            // Is the robot disabled? This is true for the DISABLED State.
            API bool disabled() {
                return _id == 0;
            }
            
            // Is the robot autonomous? This is true for the AUTONOMOUS State.
            API bool autonomous() {
                return _id == 1;
            }
            
            // Is the robot teleoperated? This is true for the TELEOP State
            API bool teleop() {
                return _id == 2;
            }
            
            // Is the robot in test mode? This is true for the TEST State.
            API bool test() {
                return _id == 3;
            }
            
            // Convert the state to a string. This is one of either "Disabled",
            // "Autonomous", "Teleop" or "Test"
            API std::string to_string() {
                if (disabled()) return "Disabled";
                if (autonomous()) return "Autonomous";
                if (teleop()) return "Teleop";
                if (test()) return "Test";
            }
            
            API int ordinal() {
                return _id;
            }
        private:
            int _id;
    };
    
    class IterativeBase {
        public:
            IterativeBase() { }
        
            API virtual void disabledInit() { };
            API virtual void disabledPeriodic() { };
            
            API virtual void autonomousInit() { };
            API virtual void autonomousPeriodic() { };
            
            API virtual void teleopInit() { };
            API virtual void teleopPeriodic() { };
            
            API virtual void testInit() { };
            API virtual void testPeriodic() { };
    };
    
    namespace States {
        namespace Internal {
            API void set_tick_timing(int ms);
            API int get_tick_timing();
            
            API void set_state(int id);
        }
        
        API State DISABLED();
        API State AUTO();
        API State TELEOP();
        API State TEST();
        
        API State from_ordinal(int id);
        
        API int current_state_ordinal();
        API int last_state_ordinal();
        
        API State current_state();
        API State last_state();
        
        API void start_tracker();
        
        // Register a tracker. Trackers are called when the robot transitions from 
        // one state to another. This accepts a function pointer of return type
        // 'void' and accepting two arguments of Toast::States::State, with the
        // first argument being the old state, and the second argument being the
        // new (current) state.
        API void register_tracker(void (*func)(State, State));
        
        // Register a ticker. Tickers are called periodically at the same frequency
        // as the IterativeRobot periodic functions. This accepts a function pointer
        // of return type 'void' and a single argument of type Toast::States::State
        // denoting the current state the robot is in.
        API void register_ticker(void (*func)(State));
        
        // Register an IterativeBase. IterativeBase is designed to mimic the layout
        // of IterativeRobot, and as such provides both Periodic and Init functions.
        API void register_iterative(IterativeBase *iterative);
    }
}