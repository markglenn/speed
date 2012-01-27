#ifndef _SELECTOR_H_
#define _SELECTOR_H_

#include <list>
#include "Socket.h"

namespace System
{
	namespace Net
	{
		class Selector
		{
		public:
			Selector(void);
			virtual ~Selector(void);

			/**
				Adds a socket to the list

				@param pSocket Socket to add to the list
			*/
			void AddSocket ( Socket* pSocket );

			/**
				Removes a socket from the list

				@param pSocket Socket to remove from the list
			*/
			void RemoveSocket ( Socket* pSocket );

			/**
				Does the select process
			*/
			void Select ( );

			// Test functions //////////////////////////////////////

			/**
				Returns true if the socket won't block on read

				@param pSocket Socket to test
				@returns True if read won't block
			*/
			bool IsReadable ( Socket *pSocket );

			/**
				Returns true if the socket won't block on write

				@param pSocket Socket to test
				@returns True if write won't block
			*/
			bool IsWritable ( Socket *pSocket );

			/**
				Returns true if the socket errored out

				@param pSocket Socket to test
				@returns True if the socket has an error
			*/
			bool IsError    ( Socket *pSocket );

		private:
			std::list <Socket *> m_vSocketList;

			fd_set m_fdRead;
			fd_set m_fdWrite;
			fd_set m_fdError;

		};
	}
}
#endif // _SELECTOR_H_
