#include "Semaphore.h"

namespace System
{
	namespace Threading
	{
		//////////////////////////////////////////////////////////////////////
		Semaphore::Semaphore( int count )
		//////////////////////////////////////////////////////////////////////
		{
#ifdef WIN32
			int max = 10;
			if ( count > max )
				max = count * 10;

			m_hSemaphore = CreateSemaphore( 
				NULL,		// no security attributes
				count,		// initial count
				max,		// maximum count
				NULL);		// unnamed semaphore
#else
			sem_init( &m_hSemaphore, 0, count );
#endif
		}

		//////////////////////////////////////////////////////////////////////
		Semaphore::~Semaphore(void)
		//////////////////////////////////////////////////////////////////////
		{
#ifdef WIN32
			CloseHandle (m_hSemaphore);
#else
			sem_destroy ( &m_hSemaphore );
#endif
		}

		//////////////////////////////////////////////////////////////////////
		bool Semaphore::Down ( )
		//////////////////////////////////////////////////////////////////////
		{
#ifdef WIN32
			return ( WaitForSingleObject ( m_hSemaphore, INFINITE ) == WAIT_OBJECT_0 );
#else
			return ( sem_wait ( &m_hSemaphore ) == 0 );
#endif
		}

		//////////////////////////////////////////////////////////////////////
		void Semaphore::Up ( )
		//////////////////////////////////////////////////////////////////////
		{
#ifdef WIN32
			ReleaseSemaphore( 
				m_hSemaphore,  // handle to semaphore
				1,             // increase count by one
				NULL);
#else
			sem_post ( &m_hSemaphore );
#endif
		}
	}
}
