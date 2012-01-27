#include "Kernel.h"
#include "../Threading/Thread.h"

using System::Threading::Thread;
namespace System
{
	namespace Engine
	{
		//////////////////////////////////////////////////////////////////
		Kernel::Kernel ( ) 
		//////////////////////////////////////////////////////////////////
		{
			_oTimer.Initialize ( );
		}

		//////////////////////////////////////////////////////////////////
		Kernel::~Kernel ( )
		//////////////////////////////////////////////////////////////////
		{

		}

		//////////////////////////////////////////////////////////////////
		void Kernel::Run ( )
		//////////////////////////////////////////////////////////////////
		{
			// Starting the kernel, need to reset the timer
			_oTimer.Reset ( );

			while ( _lServiceList.size() )
			{
				// Do once so all services get the same time per run
				_oTimer.Update ( );
				float time = _oTimer.GetTimerTime();

				// Reset so we can time only this current loop
				_oTimer.Reset ( );

				std::list< Service* >::iterator it;
				for ( it = _lServiceList.begin(); it != _lServiceList.end(); )
				{
					Service* pService = *it;

					// This is done so we can erase dead services
					it++;

					// Remove a dead service
					if ( pService->IsDead() )
					{
						pService->Stop ( );
						_lServiceList.remove ( pService );
					}
					// Update if still running
					else if ( !pService->IsSuspended ( ) )
						pService->Update ( time );

				}

				Thread::Yield ( ); 
			}
		}

		//////////////////////////////////////////////////////////////////
		bool Kernel::AddService ( Service *pService )
		//////////////////////////////////////////////////////////////////
		{
			// Try starting the service
			if ( !pService->Start ( ) )
				return false;

			// This is a priority list, add the new service in order			
			std::list< Service* >::iterator it;
			for ( it = _lServiceList.begin(); it != _lServiceList.end(); it++ )
			{
				if ( (*it)->GetPriority() > pService->GetPriority() )
				{
					_lServiceList.insert ( it, pService );
					return true;
				}
			}

			// Lowest priority, add to the end of the list
			_lServiceList.insert ( _lServiceList.end(), pService );

			return true;
		}

		//////////////////////////////////////////////////////////////////
		void Kernel::RemoveService ( Service *pService )
		//////////////////////////////////////////////////////////////////
		{
			pService->SetDead ( true );
		}

		//////////////////////////////////////////////////////////////////
		void Kernel::RemoveAllServices ( )
		//////////////////////////////////////////////////////////////////
		{
			std::list< Service* >::iterator it;
			for ( it = _lServiceList.begin(); it != _lServiceList.end(); it++ )
				(*it)->SetDead ( true );
		}

		//////////////////////////////////////////////////////////////////
		bool Kernel::SuspendService ( Service *pService )
		//////////////////////////////////////////////////////////////////
		{
			// Make sure the task wasn't already suspended
			if ( pService->IsSuspended () )
				return false;

			pService->SetSuspended ( true );
			pService->OnSuspend ( );

			return true;
		}

		//////////////////////////////////////////////////////////////////
		bool Kernel::ResumeService ( Service *pService )
		//////////////////////////////////////////////////////////////////
		{
			// Make sure the task is suspended before resuming
			if ( !pService->IsSuspended () )
				return false;

			pService->SetSuspended ( false );
			pService->OnResume ( );

			return true;
		}

	}
}
