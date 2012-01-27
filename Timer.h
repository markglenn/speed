// Timer.h: interface for the CTimer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _TIMER_H_
#define _TIMER_H_

#ifndef WIN32
#include <sys/time.h>
#endif

namespace System
{
	/**
		A 64 bit high precision timer.

		The Timer class allows for timing at the highest precision that windows and linux
		allows.  If the high precision timer is unavailable under windows, the class will 
		revert back to the standard multimedia timer.
	*/
	class Timer
	{
	public:

		/// Constructor
		Timer();

		/// Destructor
		~Timer();

		/** Initializes the timer.
			Chooses between the high performance timer and the multimedia 
			timer */
		void Initialize ();

		/** Resets  the timer to zero */
		void Reset();

		/** Call this function to update the timer value */
		void Update();

		/** Gets the current time

			@returns The current time */
		inline const float & GetCurrentTime() const { return m_fCurrentTime; }

		/** Gets the timer value

			@returns The timer value */
		inline const float & GetTimerTime()   const { return m_fTimerTime;   } 

	private:

		typedef float (Timer::* TimeUpdateFunc) ();
		TimeUpdateFunc m_pfnUpdate;

		float	m_fBaseTime;
		float	m_fLastTime;

		float	m_fCurrentTime;
		float	m_fTimerTime;

#ifdef WIN32
		//QueryPerformancesCounter related 64bit integers.
		//These are Microsoft specific, and will have to be changed for
		//different compilers.
		_int64  m_dTimerStart;
		float   m_fSecsPerTick;

		//One of these gets bound to the TimeUpdateFunc pointer
		float	GetPerformanceCounterTime();
		float	GetMMTime();

#else
		float   GetLinuxTime ();
		struct timeval m_dTimerStart;
#endif
	};
}

#endif
