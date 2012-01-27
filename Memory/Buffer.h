#ifndef _BUFFER_H_
#define _BUFFER_H_

namespace System
{
	namespace Memory
	{
		/** Interface for creating a read/write memory buffer */
		class Buffer
		{
		public:
			/** Reads a block of data from the buffer
				
				@param pData Pointer to a user memory block
				@param uiDataLength Length to read
				@param bPeek Remove the data from the buffer (Default: FALSE)
				@returns True if there is enough data available for the full read */
			virtual bool Read  (void *pData, unsigned int uiDataLength, bool bPeek) = 0;

			/** Writes a block of data to the buffer

				@param pData Pointer to the block of data to write
				@param uiDataLength Length of the data to write
				@returns True if there is enough space available for the user block */
			virtual bool Write (void *pData, unsigned int uiDataLength) = 0;

			/** Initializes the block of data of the requested length

				@param uiBufferLength Length of the buffer
				@returns True if the allocation was successful
			*/
			virtual bool Initialize (unsigned int uiBufferLength) = 0;

			/** Returns the current size of the buffer's data

				@returns The number of bytes available to read */
			virtual unsigned int Length () = 0;
		};
	}
}

#endif
