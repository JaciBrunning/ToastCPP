#pragma once

#include "toast/library.hpp"
#include "toast/environment.hpp"

#include "toast/concurrent/mutex.hpp"

namespace Toast {
	namespace Concurrent {
		class ConditionVariable {
		public:
			API ConditionVariable();
			API ConditionVariable(Concurrent::Mutex *mtx);
			API ConditionVariable(bool _stateful) : ConditionVariable() {
				stateful = _stateful;
			}
			API virtual ~ConditionVariable();

			API void set_stateful(bool status);
			API void lock() const;
			API void unlock() const;

			API void wait();
			API void signal();
			API void signal_all();

			API Mutex *get_mtx() { return _mtx; }
		protected:
			Mutex *_mtx;
			bool stateful = false, already_done = false;
#ifdef OS_WIN
			CONDITION_VARIABLE _cond;
#else
			pthread_cond_t _cond;
#endif
			bool _mtx_local;
		};
	}
}