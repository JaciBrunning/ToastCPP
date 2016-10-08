#pragma once

#include "toast/internal/shm.hpp"
#include "toast/library.hpp"

#include "shared.fwi.hpp"
#include "private.fwi.hpp"

#include "toast/concurrent/mutex.hpp"

#include <string>
#include <stdlib.h>

// Impl
namespace Toast {
    namespace Memory {
        API void initialize_bootstrap();
        API void initialize();
		API void free_memory(bool bootstrap);
        API char get_endian_bit();
        
		API SharedPool *shared();

		// This stores all of Toast's IPC Mutexes (Process-Shared Mutexes)
		// These make sure we don't try to access the same resource at once
		struct SharedMutex {
		public:
			Concurrent::IPCMutex *logger;

			Concurrent::IPCMutex *onboard_accel;
			Concurrent::IPCMutex *analog_out;
			Concurrent::IPCMutex *analog_in;
			Concurrent::IPCMutex *dio;
			Concurrent::IPCMutex *relay;
			Concurrent::IPCMutex *pcm;
			Concurrent::IPCMutex *motor;
			Concurrent::IPCMutex *servo;
			Concurrent::IPCMutex *pwm;
			Concurrent::IPCMutex *joy;
			Concurrent::IPCMutex *power;
			Concurrent::IPCMutex *ds;
		};

		API SharedMutex *shared_mutex();

        class Bridge {
            public:
                API Bridge(std::string name, int size);
                
                API void create();
                API void open();
                API char *get();
                API void destroy();
                API void close();
                API void zero();
                API int size();
            private:
                SHM_HANDLE _handle;
                std::string _name;
                int _size;
                char *_block;
        };
        
        API void copy_private_pool(int module_idx, char *buffer);
    }
}