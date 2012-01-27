#ifndef _THREADINGSYSTEMDEFS_H_
#define _THREADINGSYSTEMDEFS_H_

namespace System
{
	namespace Threading
	{
#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define RUNNABLE_RETURN		unsigned int __stdcall
		typedef HANDLE				Thread_t;
		typedef CRITICAL_SECTION	Mutex_t;
		typedef HANDLE				Semaphore_t;

#else // LINUX

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define RUNNABLE_RETURN		void *
		typedef pthread_t			Thread_t;
		typedef pthread_mutex_t		Mutex_t;
		typedef sem_t				Semaphore_t;

#endif
	}
}
#endif // _THREADINGSYSTEMDEFS_H_
