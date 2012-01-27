#ifndef _GENERIC_ISTREAM_H_
#define _GENERIC_ISTREAM_H_

#include <iostream>

namespace System
{
	namespace FileSystem
	{
		class generic_istream :	public std::istream
		{
		public:
			generic_istream(std::streambuf *streamBuf);
			virtual ~generic_istream(void);

		private:
			std::streambuf *m_pStreamBuffer;
		};
	}
}
#endif
