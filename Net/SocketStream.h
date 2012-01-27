#ifndef _SOCKETSTREAM_H_
#define _SOCKETSTREAM_H_

#include <iostream>
#include "SocketStreamBuf.h"

namespace System
{
	namespace Net
	{
		/**
			Socket I/O Stream class
		*/
		class SocketStream : public std::iostream
		{
		public:
			/**
				Creates the socket stream from a socket stream buffer

				@param pStreamBuf Stream buffer to read/write from
			*/
			SocketStream(SocketStreamBuf *pStreamBuf);

			/**
				Creates the socket stream from a socket

				@param pSocket Socket to read/write from
			*/
			SocketStream(Socket *pSocket);

			/**
				Destructor
			*/
			virtual ~SocketStream(void);

		private:
			SocketStreamBuf *m_pStreamBuffer;
		};
	}
}

#endif
