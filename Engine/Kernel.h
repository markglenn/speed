#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <list>

#include "Service.h"
#include "../Timer.h"
#include "../Threading/Runnable.h"

namespace System
{
	namespace Engine
	{
		class Kernel : public Threading::Runnable
		{
		public:
			Kernel ( );
			virtual ~Kernel ( );

			virtual void Run ( );

			bool AddService ( Service *pService );
			void RemoveService ( Service *pService );
			void RemoveAllServices ( );

			bool SuspendService ( Service *pService );
			bool ResumeService ( Service *pService );

		private:
			std::list< Service* > _lServiceList;
			System::Timer		  _oTimer;
		};
	}
}
#endif // _KERNEL_H_
