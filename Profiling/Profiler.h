#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <string>
#include <algorithm>
#include "Tree.h"
#include "../Math.h"

//#define PERFORMANCE_MONITOR_ENABLED

#ifdef PERFORMANCE_MONITOR_ENABLED
#define Profile(Name) System::Profiling::Profiler _perfromanceMonitor(Name)
#else
#define Profile(Name)
#endif

namespace System
{
	namespace Profiling
	{
		class ProfileStatistics
		{
		public:
			ProfileStatistics (const std::string &szLabel)
				: Min (0), Max (0), TotalTime (0), Samples (0)
			{
				this->Label = szLabel;
			}

			operator== (const ProfileStatistics& oStats) const
			{
				return (oStats.Label == this->Label);
			}

			std::string		Label;
			unsigned int	Min;
			unsigned int	Max;
			unsigned int	TotalTime;
			unsigned int	Samples;
		};

		class Profiler
		{
		public:
			Profiler(const char * szLabel)
				: m_uiStart (GetTicks ())
			{
				if (Statistics.size() == 0)
				{
					s_oParent = Statistics.insert (Statistics.begin(), ProfileStatistics (szLabel) );
					m_iThisNode = s_oParent;
				}
				else
				{ 
					// Check to see if we are resampling an existing proile subject.
					ProfileStatistics New(szLabel);

					tree<ProfileStatistics>::sibling_iterator itr = 
						std::find(Statistics.begin(s_oParent), Statistics.end(s_oParent), New);

					if (itr != Statistics.end(s_oParent))
						m_iThisNode = itr;
					else
					{ 
						//Profile subject did not exist, create a new one.
						s_oParent = Statistics.append_child(s_oParent, New);
						m_iThisNode = s_oParent;
					}
				}
			}

			virtual ~Profiler(void)
			{
				unsigned int uiElapsed = GetTicks() - m_uiStart;

				// Record statistics for this sampling
				m_iThisNode->Samples++;
				m_iThisNode->TotalTime += uiElapsed;

				if (m_iThisNode->Samples == 1)
				{
					m_iThisNode->Max = uiElapsed;
					m_iThisNode->Min = uiElapsed;
				}
				else
				{
					m_iThisNode->Max = Math <unsigned int>::Max (uiElapsed, m_iThisNode->Max);
					m_iThisNode->Min = Math <unsigned int>::Min (uiElapsed, m_iThisNode->Min);
				}

				// Reset parent pointer
				s_oParent = Statistics.parent (m_iThisNode);
			}

			static void Blah () 
			{
				return;
			}
		private:

			unsigned int GetTicks ()
			{
				unsigned int Elapsed;
				_asm //Taken from AMD SDK
				{
					//xor    eax, eax
					//xor    ebx, ebx
					//xor    ecx, ecx
					//xor    edx, edx
					_emit  0x0f				// CPUID
					_emit  0xa2
					_emit  0x0f				// RDTSC
					_emit  0x31
					mov    [Elapsed], eax
				}
				return Elapsed;
			}

			unsigned int m_uiStart;

		public:

			static tree<ProfileStatistics>						Statistics;

		private:

			tree<ProfileStatistics>::iterator					m_iThisNode;
			static tree<ProfileStatistics>::sibling_iterator	s_oParent;

		};

		tree<ProfileStatistics>						Profiler::Statistics;
		tree<ProfileStatistics>::sibling_iterator	Profiler::s_oParent;
	}
}

#endif
