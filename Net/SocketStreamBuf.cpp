#include "SocketStreamBuf.h"

namespace System
{
	namespace Net
	{

		///////////////////////////////////////////////////////////////////////
		SocketStreamBuf::SocketStreamBuf( Socket* pSocket )
		///////////////////////////////////////////////////////////////////////
		{
			m_pSocket = pSocket;

			// RX pointers set for no data
			setg (	m_oReceiveBuffer + 4,
					m_oReceiveBuffer + 4,
					m_oReceiveBuffer + 4 );

			// TX pointers set for empty slot
			setp (	m_oSendBuffer,
					m_oSendBuffer + ( TX_BUFFER_SIZE - 1 ) );

		}

		///////////////////////////////////////////////////////////////////////
		SocketStreamBuf::~SocketStreamBuf(void)
		///////////////////////////////////////////////////////////////////////
		{
			m_pSocket->Close ();
		}

		/////////////////////
		// Receive functions
		/////////////////////

		///////////////////////////////////////////////////////////////////////
		int SocketStreamBuf::underflow ()
		///////////////////////////////////////////////////////////////////////
		{
			// Is the read position before the end of the buffer?
			if ( gptr() && ( gptr() < egptr() ) )
				return * reinterpret_cast<unsigned char *>( gptr());

			int nNumPutBack = static_cast<int> ( gptr() - eback() );

			// Check the max putback
			if (nNumPutBack > 4)
				nNumPutBack = 4;

			// Copy the put back data into the buffer
			memcpy (m_oReceiveBuffer + (4 - nNumPutBack),
					gptr() - nNumPutBack,
					nNumPutBack);

			// Read as much as we can from the socket
			int nReceived = m_pSocket->Receive ( (unsigned char *)&m_oReceiveBuffer[4], RX_BUFFER_SIZE );

			// End of stream
			if ( nReceived == -1 )
				return EOF;

			// Set the new pointers to add in the full received data
			setg (	m_oReceiveBuffer + ( 4 - nNumPutBack ), 
					m_oReceiveBuffer + 4,
					m_oReceiveBuffer + 4 + nReceived );

			return *gptr();
		}

		//////////////////
		// Send functions
		//////////////////

		///////////////////////////////////////////////////////////////////////
		int SocketStreamBuf::overflow (int c)
		///////////////////////////////////////////////////////////////////////
		{
			if (c != EOF)
			{
				*pptr() = c;
				pbump(1);
			}

			// Try flushing the data
			if (Flush() == EOF)
				return EOF;

			return c;
		}

		///////////////////////////////////////////////////////////////////////
		int SocketStreamBuf::Flush ( )
		///////////////////////////////////////////////////////////////////////
		{
			int nCharacters = (int)( pptr() - pbase() );

			if ( m_pSocket->Send ( (unsigned char *)m_oSendBuffer, nCharacters ) 
				== SOCKET_ERROR )
				return EOF;

			pbump (-nCharacters);

			return nCharacters;
		}

		///////////////////////////////////////////////////////////////////////
		int SocketStreamBuf::sync ( )
		///////////////////////////////////////////////////////////////////////
		{
			if (Flush() == EOF)
				return -1;

			return 0;
		}
	}
}
