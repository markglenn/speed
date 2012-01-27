#include "WorkQueue.h"
#include "Thread.h"

namespace System
{
	namespace Threading
	{
		///////////////////////////////////////////////////////////////////////
		WorkQueue::WorkQueue( int poolSize )
		///////////////////////////////////////////////////////////////////////
		{
			m_pSemaphore = new Semaphore ( );
			for ( int i = 0; i < poolSize; i++ )
			{
				PThread pThread = new Thread ( new WorkQueue::WorkThread ( this ) );
				pThread->Start ( );

				m_oThreads.push_back ( pThread );
			}
		}

		///////////////////////////////////////////////////////////////////////
		WorkQueue::~WorkQueue(void)
		///////////////////////////////////////////////////////////////////////
		{
			delete m_pSemaphore;
			
			// Clear out the thread queue
			std::vector< PThread >::iterator i;
			for ( i = m_oThreads.begin(); i != m_oThreads.end(); i++ )
			{
				(*i)->Shutdown ( );
				delete (*i)->Target();
			}
		}

		///////////////////////////////////////////////////////////////////////
		PRunnable WorkQueue::GetWorkingThread ( )
		///////////////////////////////////////////////////////////////////////
		{
			// Wait until a working item is available
			if ( !m_pSemaphore || !m_pSemaphore->Down () )
				return NULL;

			Lock lock( m_oMutex );

			// Get the next item
			PRunnable pRunnable = m_oQueue.front ( );
			m_oQueue.pop ( );
			
			return pRunnable;

		}

		///////////////////////////////////////////////////////////////////////
		void WorkQueue::QueueWork ( PRunnable pWork )
		///////////////////////////////////////////////////////////////////////
		{
			Lock lock ( m_oMutex );

			m_oQueue.push ( pWork );
			m_pSemaphore->Up ( );
		}


	}
}