#pragma once

#include "toast/library.hpp"
#include "toast/util.hpp"
#include "toast/concurrent/mutex.hpp"

namespace Toast {
	namespace Command {
		struct Executor;

		struct Command {
			API virtual ~Command() = default;

			API virtual void start() {};
			API virtual void periodic() = 0;
			API virtual bool complete() { return false; };
			API virtual void stop() {};
			API virtual bool can_start() { return true; }

			API Command *then(Command *cmd, long delay = 0L);
			API Command *next();
			API long next_delay();

			API long time_passed();

			API void timeout(long milliseconds);
			API long timeout();
			API Executor *exec();

			void prelaunch(long time, Executor *exec);
		private:
			Command *_next;
			Executor *_exec;
			long _next_scheduled_millis;

			long _timeout = 0;
			long _time_launched;
		};

		enum CommandOperation {
			OP_CMD_STACK, OP_PUSH, OP_POP
		};

		typedef struct {
			bool exists;
			int operation;
			Command *cmd;
			long long timeout_start;
			long timeout_duration;
		} StackMember;

		struct Executor {
			API static Executor *instance();

			API Executor(int start_size = 10, int realloc_size = 10);

			API void push(Command *cmd, long delay = 0);
			API void push_unsafe(Command *cmd);

			API void pop(Command *cmd);
			API void pop_unsafe(Command *cmd);

			API void tick();

		private:
			int _realloc_size;

			int find_stack_index(Command *cmd, StackMember **stack, int *stack_size);
			int create_stack_index(Command *cmd, StackMember **stack, int *stack_size);

			StackMember *command_stack;
			int command_stack_size;

			StackMember *op_stack;
			int op_stack_size;

			Toast::Concurrent::Mutex mtx;
		};
	}
}