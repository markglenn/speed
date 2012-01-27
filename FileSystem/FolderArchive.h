#ifndef _FOLDERARCHIVE_H_
#define _FOLDERARCHIVE_H_

#include <fstream>
#include <string>
#include "IArchive.h"

namespace System
{
	namespace FileSystem
	{
		class FolderArchive : public IArchive
		{
		public:
			FolderArchive(std::string folderName);
			virtual ~FolderArchive(void);

			virtual std::string		GetArchiveName () { return m_szFolderName; }
			virtual PIStream		LoadFile (std::string filename);
			virtual bool			FileExists (std::string filename);
			virtual bool			Writable () { return true; }

			virtual POStream		WriteFile (std::string filename);

		private:
			std::string m_szFolderName;
		};
	}
}
#endif
