#ifndef _THREAD_H_
#define _THREAD_H_


#include "Runnable.h"
#include "ThreadingSystemDefs.h"

// Win32 has a legacy define for yield, remove it
#undef Yield

namespace System
{
	namespace Threading
	{
		/**	Thread base class for setting up a thread
			Make this class your base class when creating a thread.  Overriding
			the Thread::Run() function will give the new class a member thread function.
		*/
		class Thread : public Runnable
		{
		public:
			/** Constructor */
			Thread(Runnable *pRunnable = NULL);

			/** Destructor */
			virtual ~Thread();

			/** Starts a thread to run */
			bool Start ();

			/** Tells the thread to stop */
			void Shutdown ();

			/** Waits for the thread to finish */
			void Join ();

			/** Returns the thread ID

				@returns The thread ID */
			unsigned int ThreadID ()
			{
#ifdef WIN32
				return m_uiThreadID;
#else
				return m_hThread;
#endif
			}

			Runnable* Target ( ) { return m_pRunnableTarget; }

			/** Override this function with the threaded code */
			virtual void Run ( ) {};

			static void Yield ( ) 
			{ 
#ifdef WIN32
				Sleep ( 1 );
#else
				usleep ( 1000 );
#endif
			}

			static void Sleep ( unsigned int timems )
			{
#ifdef WIN32
				Sleep ( timems );
#else
				if ( timems >= 1000 )
					sleep ( timems / 1000 );

				usleep ( (timems % 1000) * 1000 );
#endif				
			}

		private:
			static RUNNABLE_RETURN threadFunc(void *args);
			Thread_t		m_hThread;
			
			unsigned int	m_uiThreadID;

			Runnable		*m_pRunnableTarget;
		};
		//////////////////////////////////////////////////////////////////////
	}
}

#endif
