#include "ServerListener.h"
#include "Selector.h"
#include "../Threading/Thread.h"

using System::Threading::Thread;

namespace System
{
	namespace Net
	{
		///////////////////////////////////////////////////////////////////////
		ServerListener::ServerListener(void)
		///////////////////////////////////////////////////////////////////////
		{
			m_oServerSocket.Create ();
		}

		///////////////////////////////////////////////////////////////////////
		ServerListener::~ServerListener(void)
		///////////////////////////////////////////////////////////////////////
		{
			m_oServerSocket.Close ( );
		}

		///////////////////////////////////////////////////////////////////////
		bool ServerListener::Bind ( const InetAddr &addr )
		///////////////////////////////////////////////////////////////////////
		{
			return m_oServerSocket.Bind ( addr );
		}

		///////////////////////////////////////////////////////////////////////
		void ServerListener::Run ( )
		///////////////////////////////////////////////////////////////////////
		{
			Selector selector;

			selector.AddSocket ( &m_oServerSocket );

			m_oServerSocket.Listen ( );

			while ( Running () )
			{
				// Wait for a connection
				do
				{
					Thread::Yield ();
					selector.Select ( );
				} while ( !selector.IsReadable (&m_oServerSocket) && Running() );

				// No longer running, exit out before accepting
				if ( !Running() )
					return;
				
				// Fire the on connect event
				OnClientConnect.SetSocket ( m_oServerSocket.Accept ( ) );
				OnClientConnect.Fire ( );
			}

		}
	}
}
