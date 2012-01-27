#ifndef _LISTENER_H_
#define _LISTENER_H_

namespace System
{
	namespace Events
	{
		template < typename eventClass >
		class Listener 
		{
		public:
			virtual void Handle ( eventClass* event ) = 0;
		};
	}
}
#endif // _LISTENER_H_
