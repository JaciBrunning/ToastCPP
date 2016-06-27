#pragma once

#include "toast/library.hpp"
#include "toast/environment.hpp"

#ifdef OS_WIN
	#include <windows.h>
	typedef HANDLE pthread_mutex_t;
#else
	#include <pthread.h>
#endif

namespace Toast {
	namespace Concurrent {
		class Mutex {
		public:
			Mutex();
			virtual ~Mutex();

			void lock();
			void unlock();
		protected:
			pthread_mutex_t _mutex;
		};
	}
}