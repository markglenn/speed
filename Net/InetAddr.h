#ifndef _INETADDR_H_
#define _INETADDR_H_

#include <string>
#ifdef WIN32

#include <winsock.h>

#else

#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


typedef int SOCKET;
#define INVALID_SOCKET ( -1 )
#define SOCKET_ERROR ( -1 )

#endif

namespace System
{
	namespace Net
	{
#ifdef WIN32
		typedef int socklen_t;
#endif

		class InetAddr : public sockaddr_in
		{
		public :
			/**
				Creates an InetAddr for a port

				@param wPort Port of the address
			*/
			InetAddr(short wPort = 0);

			/**
				Creates an InetAddr for a port and address

				@param szAddress Address
				@param wPort Port of the address
			*/
			InetAddr(const char* szAddress, short wPort = 0);

			/**
				Copys a network address into an InetAddr

				@param szAddress to copy
				@returns a copy of the InetAddr
			*/
			InetAddr& operator = (const char* szAddress);

			std::string IPAddress ( void );
			std::string Hostname ( void );
			unsigned int Port ( void );

		protected :

			/** 
				Resolves a network address into an IP address

				@param szAddress Network address to resolve
				@param wPort Port
			*/
			void Resolve(const char* szAddress, short wPort = 0);
		};
	}
}

#endif // _INETADDR_H_
