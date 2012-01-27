#include "stored_streambuf.h"
#include "../Math.h"

using namespace std;

namespace System
{
	namespace FileSystem
	{
		///////////////////////////////////////////////////////////////////////
		stored_streambuf::stored_streambuf 
			(string szFilename, unsigned int nOffset, unsigned int nFileLength)
		: m_nFileLength (nFileLength), m_nFilePosition (0)
		///////////////////////////////////////////////////////////////////////
		{
			// Reopen the zip file
			m_sFileStream.open (szFilename.c_str(), ios::in | ios::binary);
			m_sFileStream.seekg (nOffset, ios::beg);

			// Pointers set for no data
			setg (	m_oBuffer + 4,
					m_oBuffer + 4,
					m_oBuffer + 4);
		}

		///////////////////////////////////////////////////////////////////////
		stored_streambuf::~stored_streambuf ( )
		///////////////////////////////////////////////////////////////////////
		{
			m_sFileStream.close ();
		}

		///////////////////////////////////////////////////////////////////////
		int stored_streambuf::underflow ()
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
			memcpy (m_oBuffer + (4 - nNumPutback),
					gptr() - nNumPutback,
					nNumPutback);

			// No more data to read, EOF
			if (m_nFileLength == m_nFilePosition)
				return EOF;

			// Check how much data is still available
			int nDataAvailable = System::Math<int>::Min ( BUFFER_SIZE,
				m_nFileLength - m_nFilePosition );

			// Read in the next block of data
			m_sFileStream.read (&m_oBuffer[4], nDataAvailable);

			// Set the pointers
			setg (	m_oBuffer + (4 - nNumPutback),
					m_oBuffer + 4,
					m_oBuffer + 4 + nDataAvailable );

			m_nFilePosition += nDataAvailable;
			return *gptr();
		}
	}
}
