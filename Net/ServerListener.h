#ifndef _SERVERLISTENER_H_
#define _SERVERLISTENER_H_

#include "ServerSocket.h"
#include "../Threading/Runnable.h"
#include "ClientConnectEvent.h"

namespace System
{
	namespace Net
	{
		class ServerListener : public Threading::Runnable
		{
		public:
			ServerListener(void);
			virtual ~ServerListener(void);

			virtual void Run ( );
			bool Bind( const InetAddr& addr );

			ClientConnectEvent OnClientConnect;
		private:
			ServerSocket m_oServerSocket;
		};
	}
}

#endif // _SERVERLISTENER_H_
