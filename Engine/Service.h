#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <string>

namespace System
{
	namespace Engine
	{
		class Service
		{
		public:
			Service()
			{
				_uiPriority		= 500;
				_bIsDead		= false;
				_bIsSuspended	= false;
			};

			virtual ~Service ( ) {};

			virtual bool Start ( ) = 0;
			virtual void Update ( float duration ) = 0;
			virtual void Stop ( ) = 0;

			virtual void OnSuspend ( ) { };
			virtual void OnResume ( ) { };
			
			unsigned int GetPriority () { return _uiPriority; }
			void SetPriority ( unsigned int value ) { _uiPriority = value; }

			bool IsDead () { return _bIsDead; }
			void SetDead ( bool value ) { _bIsDead = value; }

			bool IsSuspended () { return _bIsSuspended; }
			void SetSuspended ( bool value ) { _bIsSuspended = value; }

		private:
			unsigned int _uiPriority;
			bool _bIsDead;
			bool _bIsSuspended;
		};
	}

}

#endif // _SERVICE_H_
