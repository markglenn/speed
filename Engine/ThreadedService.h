#ifndef _THREADED_SERVICE_H_
#define _THREADED_SERVICE_H_

#include "Service.h"
#include "../Threading/Thread.h"

namespace System
{
	namespace Engine
	{
		class ThreadedService : public Service, Threading::Runnable
		{
		public:
			ThreadedService();

			virtual ~ThreadedService ( );

			virtual bool Start ( );
			virtual void Update ( float duration ) { };
			virtual void Stop ( );

		protected:
			virtual void ServiceMethod ( ) = 0;
		
		private:

			virtual void Run ( );

			Threading::Thread *_pThread;

		};
	}

}

#endif // _THREADED_SERVICE_H_
