#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_

#include <bitset>

#include "../Threading/Thread.h"
#include "../Memory/CircularBuffer.h"

#ifndef DWORD
#define DWORD int
#endif

namespace System
{
	namespace Memory
	{
		template <class objectType, int iPoolSize = 1000>
		class MemoryPool
		{
		public:

			/**
				Allocates the pool for the memory buffer

				@param dwPoolSize Number of objects in the pool
				@param dwObjSize Size of the objects being stored

				@returns True if successfully allocated the pool
			*/
			static inline bool AllocatePool(DWORD dwPoolSize, DWORD dwObjSize)
			{
#ifdef WIN32
				// Try to allocate the full pool from windows memory
				ms_pMemPool = (char*)VirtualAlloc( NULL, dwPoolSize * dwObjSize, 
					MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, 
					PAGE_READWRITE );
#else
				ms_pMemPool = new char[dwPoolSize * dwObjSize];
#endif
				// Could not allocate the space
				if( !ms_pMemPool)
					return false;

				// Setup the allocation table
				ms_cbAllocationTable.Initialize (dwPoolSize * sizeof(void *));

				// Add the pointers to the table
				for (unsigned int i = 0; i < dwPoolSize; i++)
				{
					void* p = ms_pMemPool + (i * dwObjSize);
					ms_cbAllocationTable.Write (&p, sizeof (void *));
				}

				// Success
				return true;
			}

			/**
				Overridden new operator for the object.  This way the software does not
				need to be changed for allocating between the pool and normal heap memory.

				@param size Size of the allocation block

				@returns Pointer to the new object
			*/
			void* operator new( size_t size )
			{
				// Thread safe lock
				Threading::Lock lock(ms_oMutex);

				// Allocate the pool on the first run
				if ( !ms_pMemPool )
				{
					// Allocate to a 4 byte alignment
					size += (4 - (size % 4)) % 4;
		
					// Try to allocate the pool
					if ( !AllocatePool ( iPoolSize, (int)size ) )
						return NULL;
				}

				// Return the first free block of memory
				return GetFreeBlock ();
			}

			/**	
				Overridden delete operator for the object.  This way the software does not
				need to be changed for deallocating between the pool and normal heap memory.

				@param p Pointer to the object
			*/
			void operator delete( void* p )
			{
				// Allow for deleting null pointers
				if ( !p ) 
					return;

				// Thread safe lock
				Threading::Lock lock(ms_oMutex);

				ms_cbAllocationTable.Write (&p, sizeof(void *));
			}


			static inline void CleanupPool ()
			{
#ifndef WIN32
			    delete[] ms_pMemPool;
			    ms_pMemPool = NULL;
#endif
			}

		private:

			// Disallow array operations
			void *operator new[] (size_t size)	{ return NULL; }
			void operator delete[] (void *p)	{ }

			/**
				Finds a free block of memory by reading the circular 
				buffer stack.

				@returns Pointer to a free block of memory
			*/
			static inline void* GetFreeBlock ()
			{
				void* pData;

				if ( !ms_cbAllocationTable.Read (&pData, sizeof (void *)) )
					return NULL;

				return pData;
			}

			static char*					ms_pMemPool;			///< Pointer to the memory block

			static Threading::Mutex			ms_oMutex;				///< Thread safe mutex
			static Memory::CircularBuffer	ms_cbAllocationTable;	///< Memory allocation table

		};

		// Static Variables
		template <class objectType, int iPoolSize>
			Memory::CircularBuffer MemoryPool<objectType, iPoolSize>::ms_cbAllocationTable;

		template <class objectType, int iPoolSize>
			char* MemoryPool<objectType, iPoolSize>::ms_pMemPool = NULL;

		template <class objectType, int iPoolSize>
			Threading::Mutex MemoryPool<objectType, iPoolSize>::ms_oMutex;
	}
}

#endif
