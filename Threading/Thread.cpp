#ifdef WIN32

#include <process.h>

#endif

#include "Thread.h"

namespace System
{
	namespace Threading
	{
		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////
		Thread::Thread(Runnable *pRunnable)
			: m_hThread(0), m_uiThreadID(0)
		//////////////////////////////////////////////////////////////////////
		{
			if ( pRunnable )
				m_pRunnableTarget = pRunnable;
			else
				m_pRunnableTarget = this;
		}

		//////////////////////////////////////////////////////////////////////
		Thread::~Thread()
		//////////////////////////////////////////////////////////////////////
		{
			Shutdown ();
#ifdef WIN32
			CloseHandle (m_hThread);
#endif
		}

		//////////////////////////////////////////////////////////////////////
		RUNNABLE_RETURN Thread::threadFunc (void *args)
		//////////////////////////////////////////////////////////////////////
		{
			Thread *pThread = reinterpret_cast<Thread *>(args);

			// Running the process
			pThread->m_pRunnableTarget->m_bRunning = true;

			pThread->m_pRunnableTarget->Run ( );

			// Done running the process
			pThread->m_pRunnableTarget->m_bRunning = false;

			return 0;
		}

		//////////////////////////////////////////////////////////////////////
		void Thread::Join()
		//////////////////////////////////////////////////////////////////////
		{
			if ( m_hThread )
			{
#ifdef WIN32
				WaitForSingleObject (m_hThread, INFINITE);
#else
				pthread_join (m_hThread, NULL);
#endif
			}
		}

		//////////////////////////////////////////////////////////////////////
		void Thread::Shutdown()
		//////////////////////////////////////////////////////////////////////
		{
			if (m_bRunning)
			{
				m_pRunnableTarget->m_bRunning = false;
				Join ();
			}
		}

		//////////////////////////////////////////////////////////////////////
		bool Thread::Start()
		//////////////////////////////////////////////////////////////////////
		{
			if (m_hThread == 0)
			{
#ifdef WIN32
				m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, 
					threadFunc, this, 0, &m_uiThreadID));
				
				return ( m_hThread != NULL );
#else
				return (0 == pthread_create (&m_hThread, NULL, &threadFunc, (void *)this) );
#endif
			}

			return false;
		}
	}
}
