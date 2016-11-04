#include "toast/concurrent/condition.hpp"

using namespace Toast::Concurrent;

ConditionVariable::ConditionVariable() {
	_mtx = new Mutex();
	_mtx_local = true;
#ifdef OS_WIN
	InitializeConditionVariable(&_cond);
#else
	pthread_cond_init(&_cond, NULL);
#endif
}

ConditionVariable::ConditionVariable(Mutex *mtx) {
	_mtx = mtx;
	_mtx_local = false;
#ifdef OS_WIN
	InitializeConditionVariable(&_cond);
#else
	pthread_cond_init(&_cond, NULL);
#endif
}

ConditionVariable::~ConditionVariable() {
#ifdef OS_WIN
	// Windows doesn't require deletion of CONDITION_VARIABLE
#else
	pthread_cond_destroy(&_cond);
#endif
	if (_mtx_local) delete _mtx;
}

void ConditionVariable::lock() const {
	_mtx->lock();
}

void ConditionVariable::unlock() const {
	_mtx->unlock();
}

void ConditionVariable::wait() {
#ifdef OS_WIN
	SleepConditionVariableCS(&_cond, _mtx->get_cs(), INFINITE);
#else
	pthread_cond_wait(&_cond, _mtx->get_mtx());
#endif
}

void ConditionVariable::signal() {
#ifdef OS_WIN
	WakeConditionVariable(&_cond);
#else
	pthread_cond_broadcast(&_cond);
#endif
}

void ConditionVariable::signal_all() {
#ifdef OS_WIN
	WakeAllConditionVariable(&_cond);
#else
	pthread_cond_signal(&_cond);
#endif
}