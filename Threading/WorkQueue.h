#ifndef _WORKQUEUE_H_
#define _WORKQUEUE_H_

#include <queue>
#include <vector>

#include "Runnable.h"
#include "Semaphore.h"
#include "Mutex.h"
#include "../Memory/SmartPointer.h"

namespace System
{
	namespace Threading
	{
		typedef Memory::SmartPointer<Runnable>	PRunnable;
		typedef Memory::SmartPointer<Thread>	PThread;

		class WorkQueue
		{
		public:
			WorkQueue( int poolSize );
			virtual ~WorkQueue(void);
	
			void QueueWork ( PRunnable pWork );
		private:
			PRunnable GetWorkingThread ( );

			class WorkThread : public Runnable
			{
			public:
				WorkThread ( WorkQueue *pWorkQueue ) : m_pWorkQueue (pWorkQueue) {}

				virtual void Run ( )
				{
					while ( this->Running() )
					{
						PRunnable pRunnable = m_pWorkQueue->GetWorkingThread ( );

						if ( pRunnable == NULL )
							return;

						pRunnable->Run ( );
					}
				}
			private:
				WorkQueue *m_pWorkQueue;

			};
			
			bool						m_bIsRunning;

			Semaphore					*m_pSemaphore;
			Mutex						m_oMutex;
			std::queue < PRunnable >	m_oQueue;
			std::vector < PThread >		m_oThreads;
		};

	}
}
#endif