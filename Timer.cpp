// Timer.cpp: implementation of the Timer class.
//
//////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
#endif

#include <iostream>
#include "Timer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace System
{
	//////////////////////////////////////////////////////////////////
	Timer::Timer() : 
		m_pfnUpdate(0), m_fBaseTime(0.0f), m_fLastTime(0.0f),
		m_fCurrentTime(0.0f), m_fTimerTime(0.0f)
	//////////////////////////////////////////////////////////////////
	{
#ifdef WIN32
	  m_dTimerStart = 0;
	  m_fSecsPerTick = 0.0f;
#endif
	}

	//////////////////////////////////////////////////////////////////
	Timer::~Timer()
	//////////////////////////////////////////////////////////////////
	{
		m_pfnUpdate = 0;
	}

	//////////////////////////////////////////////////////////////////
	//	Initialize the timer
	//////////////////////////////////////////////////////////////////
	void Timer::Initialize()
	//////////////////////////////////////////////////////////////////
	{
#ifdef WIN32
		_int64	dTicksPerSec =0;

		if (QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&dTicksPerSec)))
		{ 
			// performance counter is available, use it instead of multimedia timer
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&m_dTimerStart));
			m_fSecsPerTick = (1.0f)/(static_cast<float>(dTicksPerSec));
			m_pfnUpdate = &Timer::GetPerformanceCounterTime;
		}
		else
		{ 
			//Use MM timer if unable to use the High Frequency timer
			m_dTimerStart = timeGetTime();
			m_fSecsPerTick = 1.0f/1000.0f;
			m_pfnUpdate = &Timer::GetMMTime;
		}
#else
		struct timezone tz;
		gettimeofday (&m_dTimerStart, &tz);		
		m_pfnUpdate = &Timer::GetLinuxTime;
		
#endif

	}

	//////////////////////////////////////////////////////////////////
	//	Update time
	//////////////////////////////////////////////////////////////////
	void Timer::Update()
	//////////////////////////////////////////////////////////////////
	{
		//The ->* operator binds the function pointer to the object pointed to by
		//the right hand pointer. which is THIS below
		m_fCurrentTime =  (this->*m_pfnUpdate)() - m_fBaseTime;
		m_fTimerTime = m_fCurrentTime - m_fLastTime;
		//m_fLastTime = m_fCurrentTime;
	}

	//////////////////////////////////////////////////////////////////
	//	Reset the Timer
	//////////////////////////////////////////////////////////////////
	void Timer::Reset()
	//////////////////////////////////////////////////////////////////
	{
		m_fBaseTime =  (this->*m_pfnUpdate)();
		m_fLastTime = m_fCurrentTime = m_fTimerTime = 0.0;
	}

#ifdef WIN32
	//////////////////////////////////////////////////////////////////
	//	Time update funcs
	//////////////////////////////////////////////////////////////////
	float Timer::GetPerformanceCounterTime()
	//////////////////////////////////////////////////////////////////
	{
	        _int64 dCurTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&dCurTime));
		return (m_fSecsPerTick * (dCurTime - m_dTimerStart));
	}

	//////////////////////////////////////////////////////////////////
	float Timer::GetMMTime()
	//////////////////////////////////////////////////////////////////
	{	return (m_fSecsPerTick * (timeGetTime()- m_dTimerStart));
	}

#else
	//////////////////////////////////////////////////////////////////
	float Timer::GetLinuxTime()
	//////////////////////////////////////////////////////////////////
	{
	  double t1, t2;
	  struct timeval tend;
	  struct timezone tz;

	  gettimeofday (&tend, &tz);
	  
	  t1 =  (double)m_dTimerStart.tv_sec + (double)m_dTimerStart.tv_usec/(1000*1000);
	  t2 =  (double)tend.tv_sec + (double)tend.tv_usec/(1000*1000);

	  return (float)(t2 - t1);
	}
#endif
}
