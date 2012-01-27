#ifndef _CLIENTCONNECTEVENT_H_
#define _CLIENTCONNECTEVENT_H_

#include "../Events/Event.h"
#include "Socket.h"

namespace System
{ 
	namespace Net
	{
		class ClientConnectEvent  : public Events::Event < ClientConnectEvent >
		{
		public:
			ClientConnectEvent( ) {}
			virtual ~ClientConnectEvent(void) {}
		
			void SetSocket ( Socket s ) { m_oSocket = s; }
			Socket GetSocket ( ) { return m_oSocket; }
		
		private:
			Socket m_oSocket;
		};
	}
}

#endif // _CLIENTCONNECTEVENT_H_
