#pragma once

// If the IO Mutexes are not disabled, wrap returns and wrap functions with a mutex lock/unlock
// NOTE: It is EXTREMELY not recommended to disable mutexes. Although it will increase performance,
// there's a very likely chance that your robot will crash at some point.
#ifndef DISABLE_IO_MUTEX
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