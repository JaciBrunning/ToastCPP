#include "toast/concurrent/mutex.hpp"

using namespace Toast::Concurrent;

#ifdef OS_WIN
static int pthread_mutex_init(pthread_mutex_t *mutex, void *unused) {
	(void)unused;
	*mutex = CreateMutex(NULL, FALSE, NULL);
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
#ifdef OS_WIN
	InitializeCriticalSection(&_cs);
#else
	_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(_mutex, NULL);
#endif
}

Mutex::~Mutex() {
#ifdef OS_WIN
	DeleteCriticalSection(&_cs);
#else
	pthread_mutex_destroy(_mutex);
	free(_mutex);
#endif
}

void Mutex::lock() {
#ifdef OS_WIN
	EnterCriticalSection(&_cs);
#else
	pthread_mutex_lock(_mutex);
#endif
}

void Mutex::unlock() {
#ifdef OS_WIN
	LeaveCriticalSection(&_cs);
#else
	pthread_mutex_unlock(_mutex);
#endif
}

bool Mutex::try_lock() {
	// Don't you love OS consistency?
#ifdef OS_WIN
	return TryEnterCriticalSection(&_cs) != 0;		// 0 = Already Locked
#else
	return pthread_mutex_trylock(_mutex) == 0;		// 0 = Not already locked
#endif
}

IPCMutex::IPCMutex(std::string name, int size, bool owner) {
	_size = size;
	_name = name;
	if (owner) {
#ifdef OS_WIN
		_mutex_ptr = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * size);
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
		_mutex_ptr = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * size);
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