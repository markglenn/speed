#ifndef _EVENT_H_
#define _EVENT_H_

#include "Listener.h"
#include <list>

namespace System
{
	namespace Events
	{

		template < typename eventClass >
		class Event 
		{
		private:
			typedef Listener < eventClass > ListenerClass;

		public:
			Event(void) {}
			virtual ~Event(void) {}

			///////////////////////////////////////////////////////////////////
			bool Listen ( ListenerClass *pListener )
			///////////////////////////////////////////////////////////////////
			{
				m_vListeners.push_back ( pListener );
				return true;
			}

			///////////////////////////////////////////////////////////////////
			bool Remove ( ListenerClass *pListener )
			///////////////////////////////////////////////////////////////////
			{
				m_vListeners.remove ( pListener );
				return true;
			}

			///////////////////////////////////////////////////////////////////
			void Fire ( )
			///////////////////////////////////////////////////////////////////
			{
				typename std::list < ListenerClass* >::iterator i;

				for ( i = m_vListeners.begin();
					i != m_vListeners.end();
					i++ )
				{
					(*i)->Handle ( (eventClass *)this );

				}
			}

		private:
			std::list < ListenerClass* > m_vListeners;
		};

	}
}
#endif // _EVENT_H_
