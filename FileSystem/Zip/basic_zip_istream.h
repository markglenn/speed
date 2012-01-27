#include <fstream>
#include <vector>
#include <string>

#include "../zlib/zlib.h"

#define BUFFER_SIZE (1024)
#define BUFFER_PLUS_PUTBACK (1028)

namespace System
{
	namespace FileSystem
	{
		namespace Zip
		{
			class zip_streambuf : public std::streambuf
			{
			public:
				zip_streambuf (std::string szFilename, unsigned int nOffset, unsigned int nFileLength);
				virtual ~zip_streambuf ();

			protected:
				virtual int underflow ();
				void LoadBuffer ();

			private:
				z_stream		m_sStreamer;
				unsigned int	m_nFileLength;
				unsigned int	m_nFilePosition;
				bool			m_bErroredOut;
				bool			m_bDataAvailable;

				std::ifstream m_sCompressedStream;

				char m_oOutputBuffer[BUFFER_PLUS_PUTBACK];
				char m_oBuffer[BUFFER_SIZE]; 

			};
		}
	}
}
