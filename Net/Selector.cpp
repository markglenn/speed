#include "Selector.h"

#include "../Math.h"

namespace System
{
	namespace Net
	{
		///////////////////////////////////////////////////////////////////////
		Selector::Selector(void)
		///////////////////////////////////////////////////////////////////////
		{
			FD_ZERO ( &m_fdRead );
			FD_ZERO ( &m_fdWrite );
			FD_ZERO ( &m_fdError );
		}

		///////////////////////////////////////////////////////////////////////
		Selector::~Selector(void)
		///////////////////////////////////////////////////////////////////////
		{
		}

		///////////////////////////////////////////////////////////////////////
		void Selector::AddSocket ( Socket* pSocket )
		///////////////////////////////////////////////////////////////////////
		{
			m_vSocketList.push_back ( pSocket );
		}

		///////////////////////////////////////////////////////////////////////
		void Selector::RemoveSocket ( Socket* pSocket )
		///////////////////////////////////////////////////////////////////////
		{
			m_vSocketList.remove ( pSocket );
		}

		///////////////////////////////////////////////////////////////////////
		void Selector::Select ( )
		///////////////////////////////////////////////////////////////////////
		{
			int maxfd = 0;

			// Clear the sets
			FD_ZERO ( &m_fdRead );
			FD_ZERO ( &m_fdWrite );
			FD_ZERO ( &m_fdError );

			std::list<Socket *>::iterator i;

			// Set each of the descriptors		
			for ( i = m_vSocketList.begin(); i != m_vSocketList.end(); i++ )
			{
				maxfd = Math<int>::Max (maxfd, (int)((SOCKET)(**i)) );

				FD_SET( **i, &m_fdRead );
				FD_SET( **i, &m_fdWrite );
				FD_SET( **i, &m_fdError );
			}

			timeval time;
			memset ( &time, 0, sizeof(timeval) );

			select (maxfd + 1, &m_fdRead, &m_fdWrite, &m_fdError, &time);
		}

		///////////////////////////////////////////////////////////////////////
		bool Selector::IsReadable ( Socket *pSocket )
		///////////////////////////////////////////////////////////////////////
		{
			return ( FD_ISSET ( *pSocket, &m_fdRead ) != 0 );
		}

		///////////////////////////////////////////////////////////////////////
		bool Selector::IsWritable ( Socket *pSocket )
		///////////////////////////////////////////////////////////////////////
		{
			return ( FD_ISSET ( *pSocket, &m_fdWrite ) != 0 );
		}

		///////////////////////////////////////////////////////////////////////
		bool Selector::IsError ( Socket *pSocket )
		///////////////////////////////////////////////////////////////////////
		{
			return ( FD_ISSET ( *pSocket, &m_fdError ) != 0 );
		}
	}
}
