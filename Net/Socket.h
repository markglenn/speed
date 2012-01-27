#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "InetAddr.h"

#define SOCKET_BACKLOG ( 5 )

namespace System
{
	namespace Net
	{

#ifdef WIN32
		class WinsockInit
		{
		public :
			WSADATA m_wsd;
			int m_iStatus;

			WinsockInit(WORD wVersionRequested = 0x0101);
			~WinsockInit();
		};
#endif

		/**
			Socket class wraps a linux/windows socket object
		*/
		class Socket
		{
		public:
			/**
				Basic constructor 
			*/
			Socket ();

			/** 
				Creates a socket object that contains a SOCKET connection

				@param s SOCKET connect
			*/
			Socket ( SOCKET s, InetAddr address );
			
			/**
				Copy constructor

				@param s Socket object to copy
			*/
			Socket ( const Socket& s );
			
			/**
				Destructor

				Closes the connection if it is owned
			*/
			virtual ~Socket ( );

			/**
				Creates a linux or windows SOCKET object

				@returns True if the socket creation was done
			*/
			bool Create();
			
			/**
				Closes an open SOCKET connection
			*/
			void Close();

			/**
				Connects the client to a remote server

				@param addr Address of the server to connect to
				@returns True if the connection was established
			*/
			bool Connect(const InetAddr& addr);

			/** 
				Sends a buffer to the connected socket

				@param buf Character buffer to send
				@param cbBuf Size of the buffer

				@returns The number of bytes sent
			*/
			int Send(const void* buf, int cbBuf);

			/**
				Sends a printf like message to the connected socket

				@param fmt printf like formatted string
				@returns The number of bytes sent
			*/
			int printf(const char* fmt, ...);

			/**
				Receives a certain amount of bytes from the connected socket

				@param buf Characer buffer to receive into
				@param cbBuf Maximum size of the buffer
				
				@returns The number of bytes received
			*/
			int Receive(void* buf, int cbBuf);

			/**
				Sets a socket option

				@param opt Option to set
				@param pBuf Value of the option
				@param cbBuf Length of pBuf

				@returns True if the socket option was set
			*/
			bool SetOpt(int opt, const char* pBuf, int cbBuf);
			
			/**
				Gets a socket option

				@param opt Option to get
				@param pBuf Value of the option
				@param cbBuf Length of pBuf

				@returns True if the socket option was read
			*/
			bool GetOpt(int opt, char* pBuf, int& cbBuf);

			/**
				Cast access operator to the SOCKET object

				@returns The linux/windows SOCKET connection
			*/
			operator SOCKET& () const { return (SOCKET&)m_oSock; }

			/**
				Returns true of the socket is open

				@returns True if the socket is valid
			*/
			operator bool() const { return m_oSock != INVALID_SOCKET; }

			/**
				Returns the current address

				@returns Address the socket is connected to
			*/
			InetAddr Address() { return m_oAddress; }

		protected :

			SOCKET		m_oSock;
			InetAddr	m_oAddress;
		};
	}
}

#endif
