#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "ThreadingSystemDefs.h"

namespace System
{
	namespace Threading
	{

		/** Contains the threading mutex

			This class allows for simple thread safe capabilities.  By
			acquiring a single mutex, the threads will wait for each other 
			to finish.
		*/
		class Mutex
		{
		public:
			/** Constructor
				Initializes the mutex */
			Mutex ();

			/** Destructor
				Deletes the mutex */
			~Mutex	();

			/** Acquires the mutex for the current thread */
			void Acquire ();

			/** Releases the mutex for the current thread */
			void Release ();

		private:
			Mutex_t m_oMutex;
		};

		
		/** Automatic lock for a Mutex 

			Helper class for locking a mutex.  The constructor acquires
			the mutex while the destructor releases it */
		class Lock
		{
		public:
			/** Constructor
				Acquires the mutex */
			Lock(Mutex &mutex) : m_mutex(mutex) { m_mutex.Acquire(); }

			/** Destructor
				Releases the held mutex */
			~Lock() { m_mutex.Release(); }

		private:
			Mutex &m_mutex;
		};
	}
}
#endif // _MUTEX_H_
