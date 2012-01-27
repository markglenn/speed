#include <memory.h>

#include "Buffer.h"
#include "CircularBuffer.h"

namespace System
{
	namespace Memory
	{
		///////////////////////////////////////////////////////////////////////
		CircularBuffer::CircularBuffer () 
			: m_pBuffer(0), m_uiBufferSize(0), m_uiCurrentLength(0),
			  m_pReadPos(0), m_pWritePos(0)
		///////////////////////////////////////////////////////////////////////
		{
		}

		///////////////////////////////////////////////////////////////////////
		CircularBuffer::~CircularBuffer ()
		///////////////////////////////////////////////////////////////////////
		{
			if (m_pBuffer)
				delete[] m_pBuffer;
		}

		///////////////////////////////////////////////////////////////////////
		bool CircularBuffer::Initialize (unsigned int uiBufferLength)
		///////////////////////////////////////////////////////////////////////
		{
			// Create the new buffer
			m_pBuffer = new unsigned char[uiBufferLength];
			m_pReadPos = m_pBuffer;
			m_pWritePos = m_pBuffer;

			// Initialize the sizes
			m_uiBufferSize = uiBufferLength;
			m_uiCurrentLength = 0;

			return (m_pBuffer != 0);
		}

		///////////////////////////////////////////////////////////////////////
		bool CircularBuffer::Write (void *pData, unsigned int uiDataLength)
		///////////////////////////////////////////////////////////////////////
		{
			unsigned char* pBufferEnd = m_pBuffer + m_uiBufferSize;

			// Check to see if there is a valid buffer created
			if (!m_pBuffer)
				return false;

			// Check to see if there is enough room to write to this buffer
			if (uiDataLength + m_uiCurrentLength > m_uiBufferSize)
				return false;

			// Can the write be done in one pass?
			if (m_pWritePos + uiDataLength <= pBufferEnd)
			{
				memcpy (m_pWritePos, pData, uiDataLength);

				// Update our pointers/sizes
				m_uiCurrentLength += uiDataLength;
				m_pWritePos += uiDataLength;

				return true;
			}
			
			// Calculate the first write length
			unsigned int uiPartialSize = (unsigned int)(pBufferEnd - m_pWritePos);

			// Copy the data in two writes
			memcpy (m_pWritePos, pData, uiPartialSize);
			memcpy (m_pBuffer, (unsigned char *)pData + uiPartialSize, uiDataLength - uiPartialSize);

			// Update our pointers/sizes
			m_uiCurrentLength += uiDataLength;
			m_pWritePos = m_pBuffer + uiDataLength - uiPartialSize;

			return true;
		}

		///////////////////////////////////////////////////////////////////////
		bool CircularBuffer::Read (void *pData, unsigned int uiDataLength, bool bPeek)
		///////////////////////////////////////////////////////////////////////
		{
			unsigned char* pBufferEnd = m_pBuffer + m_uiBufferSize;

			// Check to see if there is enough data to cover this read
			if (uiDataLength > m_uiCurrentLength)
				return false;

			// Can the read be done in one read?
			if (m_pReadPos + uiDataLength <= pBufferEnd)
			{
				memcpy (pData, m_pReadPos, uiDataLength);

				if (!bPeek)
				{
					// Update our pointers/sizes
					m_pReadPos += uiDataLength;
					m_uiCurrentLength -= uiDataLength;
				}

				return true;
			}
			
			// Calculate the first read length
			unsigned int uiPartialSize = (unsigned int)(pBufferEnd - m_pReadPos);

			// Copy the data in two writes
			memcpy (pData, m_pReadPos, uiPartialSize);
			memcpy ((unsigned char *)pData + uiPartialSize, m_pBuffer, uiDataLength - uiPartialSize);

			if (!bPeek)
			{
				// Update our pointers/sizes
				m_uiCurrentLength += uiDataLength;
				m_pWritePos = m_pBuffer + uiDataLength - uiPartialSize;
			}
			return true;
		}

	}
}
