#ifndef _EVENTREGISTRAR_H_
#define _EVENTREGISTRAR_H_

#include "../Types/Singleton.h"

namespace System
{
	namespace Events
	{
		class EventRegistrar : public System::Types::Singleton<EventRegistrar>
		{
		public:
			bool RegisterListener ( Listener
		};
	}
}

#endif // _EVENTREGISTRAR_H_