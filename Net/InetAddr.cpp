#include "InetAddr.h"

#ifdef WIN32

// Force load the WINSOCK library
#pragma comment(lib, "wsock32.lib")

#else // LINUX

typedef hostent HOSTENT;
typedef in_addr IN_ADDR;

#endif

namespace System
{
	namespace Net
	{

		//////////////////////////////////////////////////////////////////////////
		InetAddr::InetAddr(short wPort)
		//////////////////////////////////////////////////////////////////////////
		{
			memset(this, 0, sizeof(sockaddr_in));
			sin_family = AF_INET;
			sin_addr.s_addr = htonl(INADDR_ANY);
			sin_port = htons((unsigned short)wPort);
		}

		//////////////////////////////////////////////////////////////////////////
		InetAddr::InetAddr(const char* szAddress, short wPort)
		//////////////////////////////////////////////////////////////////////////
		{
			Resolve(szAddress, wPort);
		}

		//////////////////////////////////////////////////////////////////////////
		InetAddr& InetAddr::operator = (const char* szAddress)
		//////////////////////////////////////////////////////////////////////////
		{
			Resolve(szAddress);
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		void InetAddr::Resolve(const char* szAddress, short wPort)
		//////////////////////////////////////////////////////////////////////////
		{
			memset(this, 0, sizeof(sockaddr_in));
			sin_family = AF_INET;
			sin_addr.s_addr = inet_addr(szAddress);

			// Only check outbound addresses
			if( sin_addr.s_addr == INADDR_NONE && strcmp(szAddress, "255.255.255.255") !=0 )
			{
				// Get the host ip from the hostname
				HOSTENT* lphost = gethostbyname(szAddress);
				
				if( lphost ) // Found
					sin_addr.s_addr = ((IN_ADDR*)lphost->h_addr)->s_addr;
				else // Not found
					sin_addr.s_addr = INADDR_ANY;
			}
			sin_port = htons((unsigned short)wPort);
		}

		//////////////////////////////////////////////////////////////////////////
		std::string InetAddr::IPAddress ( )
		//////////////////////////////////////////////////////////////////////////
		{
			return std::string( inet_ntoa (this->sin_addr) );
		}
		
		//////////////////////////////////////////////////////////////////////////
		std::string InetAddr::Hostname ( )
		//////////////////////////////////////////////////////////////////////////
		{
			HOSTENT *lphost = gethostbyaddr((char *)(&this->sin_addr), 4, AF_INET);
			
			if ( lphost )
				return std::string( lphost->h_name );
			else
				return this->IPAddress();
		}


		//////////////////////////////////////////////////////////////////////////
		unsigned int InetAddr::Port ( )
		//////////////////////////////////////////////////////////////////////////
		{
			return this->sin_port;
		}
	}
}
