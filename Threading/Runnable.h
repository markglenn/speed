#ifndef _RUNNABLE_H_
#define _RUNNABLE_H_

namespace System
{
	namespace Threading
	{
		class Runnable
		{
			friend class Thread;

		public:
			virtual void Run ( ) = 0;
			
			bool Running () { return m_bRunning; }

		private:
			bool m_bRunning;
		};
	}
}

#endif // _RUNNABLE_H_
