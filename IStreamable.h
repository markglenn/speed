#ifndef _ISTREAMABLE_H_
#define _ISTREAMABLE_H_

#include <iostream>

namespace System
{
	/**
		Streamable interface.  Subclass from this to allow simple
		streaming.
	*/
	class IStreamable 
	{
	public:
		/**
			Streams data out

			@param strm Output Stream
		*/
		virtual void StreamOut (std::ostream& strm) const = 0;

		/**
			Streams data in

			@param strm Input Stream
		*/
		virtual void StreamIn (std::istream& strm) = 0;
	};
}

/** 
	Calls the objects StreamOut when used in C++ streaming commands

	@param strm Output stream
	@param o Streamed object

	@returns The updated output stream
*/
inline std::ostream& operator << ( std::ostream& strm, const System::IStreamable& o )
{
	o.StreamOut ( strm );
	return strm;
}


/** 
	Calls the objects StreamIn when used in C++ streaming commands

	@param strm Input stream
	@param o Streamed object

	@returns The updated input stream
*/
inline std::istream& operator >> ( std::istream& strm, System::IStreamable& o )
{
	o.StreamIn ( strm );
	return strm;
}

#endif // _ISTREAMABLE_H_
