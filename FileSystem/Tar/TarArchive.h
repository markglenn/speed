#ifndef _TARARCHIVE_H_
#define _TARARCHIVE_H_

#include <string>
#include <vector>

#include "../IArchive.h"

#define	 TAR_RECORDSIZE	 512
#define	 TAR_NAMSIZ		 100
#define	 TAR_TUNMLEN	  32
#define	 TAR_TGNMLEN	  32

namespace System
{
	namespace FileSystem
	{
		namespace Tar
		{
			class TarArchive : public IArchive
			{
			public:
				TarArchive(std::string tarFile);
				virtual	~TarArchive(void);

				bool Open (std::string tarFile);

				// IArchive defines
				virtual std::string		GetArchiveName () { return m_szTarFilename; }
				virtual PIStream		LoadFile (std::string filename);
				virtual bool			FileExists (std::string filename);
				virtual bool			Writable () { return false; }

			private:
#ifdef WIN32
				#pragma pack(push)
#endif
				#pragma pack(2)
				struct TarRecord 
				{
					char name[TAR_NAMSIZ];
					char mode[8];
					char uid[8];
					char gid[8];
					char size[12];
					char mtime[12];
					char chksum[8];
					char linkflag;
					char linkname[TAR_NAMSIZ];
					char magic[8];
					char uname[TAR_TUNMLEN];
					char gname[TAR_TGNMLEN];
					char devmajor[8];
					char devminor[8];

					char dummy[167];
				};

			// GCC doesn't support push/pop, assume 32bit system
#ifdef WIN32
				#pragma pack(pop)
#else			
				#pragma pack(4)
#endif
				struct TarDirectoryEntry
				{
					std::string			szFilename;
					unsigned long		ulOffset;
					unsigned long		ulFileLength;
				};

				std::string m_szTarFilename;
				std::vector <TarDirectoryEntry> m_vTarDirectory;

			};

		}
	}
}

#endif // _TARARCHIVE_H_
