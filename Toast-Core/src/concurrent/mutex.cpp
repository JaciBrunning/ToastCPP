#include "toast/concurrent/mutex.hpp"

using namespace Toast::Concurrent;

#ifdef OS_WIN
static int pthread_mutex_init(pthread_mutex_t *mutex, void *unused) {
	(void)unused;
	auto temp_mtx = CreateMutex(NULL, FALSE, NULL);
	mutex = &temp_mtx;
	return *mutex == NULL ? -1 : 0;
}

static int pthread_mutex_destroy(pthread_mutex_t *mutex) {
	return CloseHandle(*mutex) == 0 ? -1 : 0;
}

static int pthread_mutex_lock(pthread_mutex_t *mutex) {
	return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0 ? 0 : -1;
}

static int pthread_mutex_unlock(pthread_mutex_t *mutex) {
	return ReleaseMutex(*mutex) == 0 ? -1 : 0;
}
#endif

Mutex::Mutex() {
#ifndef OS_WIN
	_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
#endif
	pthread_mutex_init(_mutex, NULL);
	mine = true;
}

Mutex::Mutex(pthread_mutex_t *_mtx, bool isnew) {
	_mutex = _mtx;
	mine = false;
	if (isnew) {
		pthread_mutex_init(_mutex, NULL);
	}
}

Mutex::~Mutex() {
	if (mine) pthread_mutex_destroy(_mutex);
}

void Mutex::lock() {
	pthread_mutex_lock(_mutex);
}

void Mutex::unlock() {
	pthread_mutex_unlock(_mutex);
}

IPCMutex::IPCMutex(std::string name, int size, bool owner) {
	_size = size;
	_name = name;
	if (owner) {
#ifdef OS_WIN
		for (int id = 0; id < size; id++)
			*(_mutex_ptr + id) = CreateMutex(NULL, FALSE, ("Global\\TOAST_IPC_MUTEX_" + name + "_" + std::to_string(id)).c_str());
#else
		handle = Toast::Internal::SHM::create_shm_file("IPC_MUTEX_" + name, size * sizeof(pthread_mutex_t));
		_mutex_ptr = (pthread_mutex_t *)Toast::Internal::SHM::map_shm_file(handle, size * sizeof(pthread_mutex_t));

		for (int id = 0; id < size; id++) {
			pthread_mutexattr_t mutex_attr;
			pthread_mutexattr_init(&mutex_attr);
			pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
			pthread_mutex_init(_mutex_ptr + id, &mutex_attr);
		}
#endif
	}
	else {
#ifdef OS_WIN
		for (int id = 0; id < size; id++)
			*(_mutex_ptr + id) = OpenMutex(SYNCHRONIZE, FALSE, ("Global\\TOAST_IPC_MUTEX_" + name + "_" + std::to_string(id)).c_str());
#else
		handle = Toast::Internal::SHM::open_shm_file("IPC_MUTEX_" + name);
		_mutex_ptr = (pthread_mutex_t *)Toast::Internal::SHM::map_shm_file(handle, size * sizeof(pthread_mutex_t));
#endif
	}
}

IPCMutex::~IPCMutex() {
	free();
}

void IPCMutex::free() {
#ifdef OS_WIN
	for (int id = 0; id < _size; id++)
		CloseHandle(*(_mutex_ptr + id));
#else
	Toast::Internal::SHM::close_shm_file("IPC_MUTEX_" + _name, handle);
#endif
}

void IPCMutex::lock(int id) {
	pthread_mutex_lock(_mutex_ptr + id);
}

void IPCMutex::unlock(int id) {
	pthread_mutex_unlock(_mutex_ptr + id);
}