#ifndef _SMARTPOINTER_H_
#define _SMARTPOINTER_H_

#ifndef __forceinline
#define __forceinline inline
#endif

namespace System
{
	namespace Memory
	{
		/***
			Smart Pointer class

			A reference counting smart pointer.  Use just like a standard pointer,
			except do not delete.  Once all references are gone, the pointer will
			delete itself.

\code
SmartPointer<MyClass> pMyObject = new MyClass ();
pMyObject->MyFunction ();
\endcode
		*/
		template <class T>
		class SmartPointer
		{

		public:
			/** Constructor

				Takes a standard pointer and stores it as its own.

				@param p Pointer value
			*/
			SmartPointer (T* p = 0) : m_pCounter(0)
			{
				if (p) 
					m_pCounter = new counter(p); 
			}

			/** Constructor

				Copies a smartpointer by adding to the reference count.

				@param r Smart pointer value
			*/
			SmartPointer (const SmartPointer& r)
			{
				acquire(r.m_pCounter);
			}

			/** Deconstructor 
				
				Causes the reference to decrease.
			*/
			virtual ~SmartPointer ()
			{
				release ();
			}

			/** 
				Copies a smart pointer by adding to the reference count.

				@param r Smart pointer value

				@returns A reference to this smartpointer (Mutable)
			*/
			SmartPointer& operator=(const SmartPointer& r)
			{
				if (this != &r)
				{
					release();
					acquire(r.m_pCounter);
				}
				return *this;
			}

			/** 
				Copies a pointer.  Takes control of it.

				@param r Pointer value

				@returns A reference to this smartpointer (Mutable)
			*/
			SmartPointer& operator=(const T* r)
			{
				if (m_pCounter && m_pCounter->ptr != r)
				{
					release();
					if (r) m_pCounter = new counter((T*)r);
				}
				return *this;
			}

			/**
				Overridden access operator.

				Allows *(pPointer) access like a standard pointer.

				@returns A reference to the object being pointed to
			*/
			T& operator*()	const
			{
				return *m_pCounter->ptr;
			}

			/**
				Overridden access operator.

				Allows pPointer->MyFunc() access like a standard pointer.

				@returns The original pointer
			*/
			__forceinline T* operator->()	const
			{
				return m_pCounter->ptr;
			}

			/**
				Compares references

				@param r Smart pointer to compare to.

				@returns True if the two smartpointers have the same reference
			*/
			bool operator==(const SmartPointer& r)
			{
				return (r.m_pCounter == m_pCounter);
			}

			/**
				Compares pointer

				@param r Pointer to compare to.

				@returns True if the two pointers are the same
			*/
			bool operator==(const T* r)
			{
				if (m_pCounter == NULL)
					return r == NULL;

				return (r == m_pCounter->ptr);
			}

			/**
				Compares references

				@param r Smart pointer to compare to.

				@returns False if the two smartpointers have the same reference
			*/
			bool operator!=(const SmartPointer& r)
			{
				return (r.m_pCounter != m_pCounter);
			}

			/**
				Compares pointer

				@param r Pointer to compare to.

				@returns False if the two pointers are the same
			*/
			bool operator!=(const T* r)
			{
				if (m_pCounter)
					return (r != m_pCounter->ptr);
				return (r != NULL);
			}

		private:
			struct counter
			{
				counter (T* p = 0, unsigned c = 1)
					: ptr (p), count(c) {}

					T*				ptr;
					unsigned int	count;
			} *m_pCounter;

			///////////////////////////////////////////////////////////////////////
			void acquire (counter *c)
			///////////////////////////////////////////////////////////////////////
			{
				m_pCounter = c;
				if (c) (c->count)++;
			}

			///////////////////////////////////////////////////////////////////////
			void release ()
			///////////////////////////////////////////////////////////////////////
			{
				if (m_pCounter)
				{
					if (--m_pCounter->count == 0)
					{
						delete m_pCounter->ptr;
						delete m_pCounter;
					}
					m_pCounter = 0;
				}
			}
		};
	}
}

#endif
