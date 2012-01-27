#ifndef _IARCHIVE_H_
#define _IARCHIVE_H_


#include <iostream>
#include <string>

#include "../Memory/SmartPointer.h"

namespace System
{
	namespace FileSystem
	{
		typedef System::Memory::SmartPointer<std::istream> PIStream;
		typedef System::Memory::SmartPointer<std::ostream> POStream;

			/**
				A very basic exception class.

				Any exception thrown is thrown with this class.  This allows the application
				to not rely on the Windows API exception.  It also allows the application to throw
				a generic exception with just an error message.

		\code
		try
		{
			throw Exception ("Error Message");
		}
		catch (Exception &e)
		{
			cout << e.what() << endl;
		}
		\endcode
			*/

		class IArchive
		{
		public:
			virtual std::string		GetArchiveName () = 0;
			virtual PIStream		LoadFile (std::string filename) = 0;
			virtual bool			FileExists (std::string filename) = 0;

			virtual bool			Writable () = 0;
			virtual POStream		WriteFile (std::string filename) { return NULL; }
		};
	}
}
#endif
