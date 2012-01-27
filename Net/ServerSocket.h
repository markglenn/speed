#ifndef _SERVERSOCKET_H_
#define _SERVERSOCKET_H_

#include "Socket.h"

namespace System
{ 
	namespace Net
	{
		class ServerSocket : public Socket
		{
		public:
			ServerSocket(void);
			virtual ~ServerSocket(void);

			/**
				Binds the SOCKET to a port or address.

				@param addr Port and/or address to bind to
				@returns True if the server was able to bind to the port and/or address
			*/
			bool Bind(const InetAddr& addr);

			/**
				Starts listening for a connection
				@returns True if the listen was successful
			*/
			bool Listen( );

			/**
				Accepts a waiting connection

				@param addr Address of the client connnecting
				@returns Socket connection to the client
			*/
			Socket Accept( );

		};
	}
}

#endif // _SERVERSOCKET_H_
