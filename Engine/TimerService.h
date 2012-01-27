#ifndef _TIMER_SERVICE_H_
#define _TIMER_SERVICE_H_

#include <map>
#include "Service.h"

namespace System
{
	namespace Engine
	{
		class TimerService : public Service
		{
		public:
			TimerService();

			virtual ~TimerService ( );

			virtual void Update ( float duration );
			virtual void Timeout ( int id ) = 0;

			void SetTimer ( int id, int duration );
			void RemoveTimer ( int id );

		private:

			typedef struct
			{
				float timeout;
				float currentTime;
			} STimerItem;

			std::map < int, STimerItem > _timerList;

		};
	}
}

#endif // _TIMER_SERVICE_H_
