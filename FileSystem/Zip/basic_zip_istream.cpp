#include "basic_zip_istream.h"
#include "../../Math.h"
#include "../stored_streambuf.h"

using namespace std;

namespace System
{
	namespace FileSystem
	{
		namespace Zip
		{
			///////////////////////////////////////////////////////////////////////
			zip_streambuf::zip_streambuf (string szFilename, unsigned int nOffset, unsigned int nFileLength)
				:	m_nFileLength (nFileLength), m_nFilePosition (0), m_bErroredOut (0),
				m_bDataAvailable (false)
			///////////////////////////////////////////////////////////////////////
			{
				// Initialize the streamer
				memset (&m_sStreamer, 0, sizeof (z_stream));
				if (inflateInit2 (&m_sStreamer, -MAX_WBITS) != Z_OK)
					return;

				// Reopen the zip file
				m_sCompressedStream.open (szFilename.c_str(), ios::in | ios::binary);
				m_sCompressedStream.seekg (nOffset, ios::beg);

				setg (	m_oOutputBuffer + 4,
						m_oOutputBuffer + 4,
						m_oOutputBuffer + 4);
			}

			///////////////////////////////////////////////////////////////////////
			zip_streambuf::~zip_streambuf ( )
			///////////////////////////////////////////////////////////////////////
			{
				m_sCompressedStream.close ();
				inflateEnd (&m_sStreamer);
			}

			///////////////////////////////////////////////////////////////////////
			int zip_streambuf::underflow ()
			///////////////////////////////////////////////////////////////////////
			{
				// Is the read position before the end of the buffer?
				if ( gptr() && ( gptr() < egptr() ) )
					return * reinterpret_cast<unsigned char *>( gptr());

				int nNumPutback = static_cast<int> ( gptr() - eback() );

				// Check the max putback
				if (nNumPutback > 4)
					nNumPutback = 4;

				// Copy the put back data into the buffer
				memcpy (m_oOutputBuffer + (4 - nNumPutback),
						gptr() - nNumPutback,
						nNumPutback);

				// No more data to read, EOF
				if (m_nFileLength == m_nFilePosition && !m_bDataAvailable)
					return EOF;

				// Load the buffer until we have some actual data in it
				do	
				{
					LoadBuffer ();
				} while (m_sStreamer.avail_out == BUFFER_SIZE && !m_bErroredOut);

				setg (	m_oOutputBuffer + (4 - nNumPutback),
						m_oOutputBuffer + 4,
						m_oOutputBuffer + 4 + (BUFFER_SIZE - m_sStreamer.avail_out) );

				return *gptr();
			}

			///////////////////////////////////////////////////////////////////////
			void zip_streambuf::LoadBuffer ()
			///////////////////////////////////////////////////////////////////////
			{
				// Only push more data onto the in buffer if the output stream is empty
				if (!m_bDataAvailable)
				{
					m_sStreamer.avail_in = System::Math<int>::Min (BUFFER_SIZE, 
						m_nFileLength - m_nFilePosition);
					m_sCompressedStream.read (m_oBuffer, m_sStreamer.avail_in);

					m_nFilePosition += m_sStreamer.avail_in;
					m_sStreamer.next_in		= (Bytef *)(m_oBuffer);
				}

				// Buffer should be completely empty for this
				m_sStreamer.next_out = (Bytef *)(&m_oOutputBuffer[4]);
				m_sStreamer.avail_out = BUFFER_SIZE;

				// Reset the data available bit
				m_bDataAvailable = false;
				
				switch ( inflate(&m_sStreamer, Z_SYNC_FLUSH) )
				{
					case Z_NEED_DICT:
					case Z_DATA_ERROR:
					case Z_MEM_ERROR:
						(void)inflateEnd(&m_sStreamer);
						m_bErroredOut = true;
						
						break;

					case Z_OK:
						// More uncompressed data is available
						m_bDataAvailable = true;
						break;
				}
			}
		}
	}
}
