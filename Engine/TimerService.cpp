#include "TimerService.h"

namespace System
{
	namespace Engine
	{
		///////////////////////////////////////////////////////////////////
		TimerService::TimerService ( )
		///////////////////////////////////////////////////////////////////
		{

		}

		///////////////////////////////////////////////////////////////////
		TimerService::~TimerService ( )
		///////////////////////////////////////////////////////////////////
		{

		}

		///////////////////////////////////////////////////////////////////
		void TimerService::Update ( float duration )
		///////////////////////////////////////////////////////////////////
		{
			std::map < int, STimerItem >::iterator i;
			for ( i = _timerList.begin(); i != _timerList.end(); i++ )
			{
				// The timer item
				STimerItem *item = &((*i).second);
					
				// Current time is in ms, duration is in us.
				item->currentTime += duration * 1000.0f;

				// Timeout occurred
				if ( item->currentTime > item->timeout )
				{
					item->currentTime -= item->timeout;
					this->Timeout ( (*i).first );
				}
			}
		}

		///////////////////////////////////////////////////////////////////
		void TimerService::SetTimer ( int id, int duration )
		///////////////////////////////////////////////////////////////////
		{
			STimerItem item = { 
				timeout: (float)duration, 
				currentTime: 0.0f };

			_timerList.insert ( std::pair <int, STimerItem> (id, item) );
		}

		///////////////////////////////////////////////////////////////////
		void TimerService::RemoveTimer ( int id )
		///////////////////////////////////////////////////////////////////
		{
			_timerList.erase ( id );
		}
	}

}
