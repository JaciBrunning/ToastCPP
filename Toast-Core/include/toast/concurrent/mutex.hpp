#pragma once

#include "toast/library.hpp"
#include "toast/environment.hpp"

#ifdef OS_WIN
	#include <windows.h>
	typedef HANDLE pthread_mutex_t;
#else
	#include <pthread.h>
	#include "toast/internal/shm.hpp"
#endif

// If the Mutexes are not disabled, wrap returns and wrap functions with a mutex lock/unlock
// NOTE: It is EXTREMELY not recommended to disable mutexes. Although it will increase performance,
// there's a very likely chance that your robot will crash at some point.
#ifndef DISABLE_MUTEX
	// Lock a mutex, get a value, unlock the mutex, return said value.
	// Put in a macro because it's extremely repetitive.
	#define MTX_RETURN(mutex, id, exp) mutex->lock(id); auto val = exp; mutex->unlock(id); return val

	// Lock a mutex, evaluate expression, unlock the mutex.
	// Put in a macro because it's extremely repetitive.
	#define MTX_WRAP(mutex, id, exp) mutex->lock(id); exp; mutex->unlock(id)

	#define MTX_LOCK(mutex, id) mutex->lock(id)
	#define MTX_UNLOCK(mutex, id) mutex->unlock(id)
#else
	#define MTX_RETURN(mutex, id, exp) return exp
	#define MTX_WRAP(mutex, id, exp) exp
	#define MTX_LOCK(mutex, id)
	#define MTX_UNLOCK(mutex, id)
#endif

namespace Toast {
	namespace Concurrent {
		class Mutex {
		public:
			API Mutex();
			API Mutex(pthread_mutex_t *_mtx, bool isnew = false);

			API virtual ~Mutex();

			API void lock();
			API void unlock();
		protected:
			pthread_mutex_t *_mutex;
			bool mine;
		};

		// In windows, this is a named mutex. In POSIX, this is a process-shared mutex from
		// pthreads, stored in a shared memory reference.
		// This is really a group of IPC Mutexes, to save on shared memory allocations on POSIX.
		class IPCMutex {
		public:
			API IPCMutex(std::string name, int group_size, bool owner);
			API virtual ~IPCMutex();

			API void free();

			API void lock(int id);
			API void unlock(int id);
		protected:
			pthread_mutex_t *_mutex_ptr;
			int _size;
			std::string _name;
			#ifndef OS_WIN
				SHM_HANDLE handle;
			#endif	
		};
	}
}