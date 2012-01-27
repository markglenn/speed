#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "ThreadingSystemDefs.h"

namespace System
{
	namespace Threading
	{
		class Semaphore
		{
		public:
			Semaphore( int count = 0 );
			virtual ~Semaphore(void);

			bool Down ( );
			void Up ( );

		private:
			Semaphore_t	m_hSemaphore;
		};
	}
}

#endif // _SEMAPHORE_H_
