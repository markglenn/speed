#ifndef _STORED_STREAMBUF_H_
#define _STORED_STREAMBUF_H_

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

//typdef typename traits_type::int_type int_type;

#ifndef BUFFER_SIZE
#define BUFFER_SIZE (1024)
#define BUFFER_PLUS_PUTBACK (1028)
#endif

namespace System
{
	namespace FileSystem
	{
		class stored_streambuf : public std::streambuf
		{
		public:
			stored_streambuf (std::string szFilename, unsigned int nOffset, unsigned int nFileLength);
			virtual ~stored_streambuf ();

		protected:
			virtual int underflow ();
			void LoadBuffer ();

		private:
			unsigned int	m_nFileLength;
			unsigned int	m_nFilePosition;
			std::ifstream	m_sFileStream;

			char m_oBuffer[BUFFER_PLUS_PUTBACK];
		};
	}
}

#endif
