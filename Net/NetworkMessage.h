#ifndef _NETWORKMESSAGE_H_
#define _NETWORKMESSAGE_H_

#include "Socket.h"
#include "NetworkMessageMaker.h"

namespace System
{
	namespace Net
	{
		class NetworkMessage 
		{
		public:
			int GetMessageID () { return m_iMessageID; }

			virtual bool SocketOut ( Socket* pSocket ) = 0;
			virtual bool SocketIn ( Socket* pSocket ) = 0;

		protected:
			int m_iMessageID;
		};
	}
}

#endif // _NETWORKMESSAGE_H_
