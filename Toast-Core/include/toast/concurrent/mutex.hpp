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