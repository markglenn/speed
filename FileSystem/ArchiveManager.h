#ifndef _ARCHIVEMANAGER_H_
#define _ARCHIVEMANAGER_H_

#include <fstream>
#include <string>
#include <vector>

#include "IArchive.h"
#include "../Memory/SmartPointer.h"
#include "../Types/Singleton.h"

namespace System
{
	namespace FileSystem
	{
		class ArchiveManager : public IArchive, System::Types::Singleton<ArchiveManager>
		{
		public:
			typedef System::Memory::SmartPointer <IArchive> PArchive;
			typedef std::vector <PArchive> ArchiveList;

			ArchiveManager();
			virtual ~ArchiveManager(void);

			virtual std::string	GetArchiveName () { return ""; }
			virtual PIStream	LoadFile (std::string filename);
			virtual bool		FileExists (std::string filename);
			virtual bool		Writable () { return true; }
			virtual POStream	WriteFile (std::string filename);

			// Composite
			void AddArchive (PArchive pArchive);
			void RemoveArchive (PArchive pArchive);
			void RemoveAllArchives ();

			static std::string ConvertPath (std::string path);

		protected:
			std::string GetRelativePath (std::string fullPath, PArchive archive);

		private:
			ArchiveList m_vArchiveList;
		};
	}
}

#endif
