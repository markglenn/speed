#ifndef _SOCKETSTREAMBUF_H_
#define _SOCKETSTREAMBUF_H_

#include <iostream>
#include "Socket.h"

#define RX_BUFFER_SIZE (1024)
#define RX_BUFFER_PLUS_PUTBACK (1028)

#define TX_BUFFER_SIZE ( 1024 )

namespace System
{
	namespace Net
	{
		class SocketStreamBuf : public std::streambuf
		{
		public:
			SocketStreamBuf( Socket *pSocket );
			virtual ~SocketStreamBuf(void);

			int Flush ();
		protected:
			virtual int underflow ( );
			virtual int overflow (int c);
			virtual int sync ( );

		private:

			char m_oReceiveBuffer[RX_BUFFER_PLUS_PUTBACK];
			char m_oSendBuffer[TX_BUFFER_SIZE];

			Socket *m_pSocket;
		};
	}
}

#endif
