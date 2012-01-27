#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

#include "Buffer.h"

namespace System
{
	namespace Memory
	{
		/**
			A continuous read/write stream buffer.

			After initializing the buffer to the maximum size, continuous read/writes
			may be made without worries about looping through the buffer.
		*/
		class CircularBuffer : Buffer
		{
		public:
			/** Constructor */
			CircularBuffer(void);

			/** Destructor */
			virtual ~CircularBuffer(void);

			/** Reads a block of data from the buffer
				
				@param pData Pointer to a user memory block
				@param uiDataLength Length to read
				@param bPeek Remove the data from the buffer (Default: FALSE)
				@returns True if there is enough data available for the full read */
			virtual bool Read  (void *pData, unsigned int uiDataLength, bool bPeek = false);

			/** Writes a block of data to the buffer

				@param pData Pointer to the block of data to write
				@param uiDataLength Length of the data to write
				@returns True if there is enough space available for the user block */
			virtual bool Write (void *pData, unsigned int uiDataLength);

			/** Initializes the block of data of the requested length

				@param uiBufferLength Length of the buffer
				@returns True if the allocation was successful
			*/
			virtual bool Initialize (unsigned int uiBufferLength);

			/** Returns the current size of the buffer's data

				@returns The number of bytes available to read */
			virtual unsigned int Length () { return m_uiCurrentLength; }

		protected:
			unsigned char *	m_pBuffer;			///< The buffer
			unsigned int	m_uiBufferSize;		///< The size of the full buffer
			unsigned int	m_uiCurrentLength;	///< The number of bytes written to the buffer

			unsigned char *	m_pReadPos;			///< Pointer to the read position
			unsigned char *	m_pWritePos;		///< Pointer to the write position
		};
	}
}

#endif
