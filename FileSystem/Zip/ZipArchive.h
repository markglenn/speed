#ifndef _ZIPARCHIVE_H_
#define _ZIPARCHIVE_H_

#include "../IArchive.h"
//#include "../ZipStream/zipstream.h"

#define ZIP_SIGNATURE 0x04034B50
#define ZIP_DIRECTORY_SIGNATURE 0x02014b50

namespace System
{
	namespace FileSystem
	{
		namespace Zip
		{

			class ZipArchive : public IArchive
			{
			public:
				ZipArchive(std::string szZipName);
				virtual ~ZipArchive(void);

				virtual std::string	GetArchiveName () { return m_szZipFile; }
				virtual PIStream	LoadFile (std::string filename);
				virtual bool		FileExists (std::string filename);
				virtual bool		Writable () { return false; }

				bool Open (std::string zipFile);

			private:
				std::string m_szZipFile;

				typedef enum
				{
					ZIP_COMPRESSION_NONE,
					ZIP_COMPRESSION_SHRINK,
					ZIP_COMPRESSION_FACTOR_1,
					ZIP_COMPRESSION_FACTOR_2,
					ZIP_COMPRESSION_FACTOR_3,
					ZIP_COMPRESSION_FACTOR_4,
					ZIP_COMPRESSION_IMPLODED,
					ZIP_COMPRESSION_TOKENIZED,
					ZIP_COMPRESSION_DEFLATE,
					ZIP_COMPRESSION_ENHANCED,
					ZIP_COMPRESSION_PKWARE
				} ECompressionMethod;

				struct ZipDirectoryEntry
				{
					std::string			szFilename;
					unsigned long		ulOffset;
					unsigned long		ulFileLength;

					ECompressionMethod	eCompression;
				};
#ifdef WIN32
				#pragma pack(push)
#endif			
				#pragma pack(2)
				struct ZipFileHeader 
				{
					unsigned int	uiSignature;
					unsigned short	usVersion;
					unsigned short	usVersionToExtract;
					unsigned short	usBitFlag;
					unsigned short	usCompressionMethod;
					unsigned short	usLastModFileTime;
					unsigned short	usLastModFileDate;
					unsigned int	uiCRC32;
					unsigned int	uiCompressedSize;
					unsigned int	uiUncompressedSize;
					unsigned short	usFilenameLength;
					unsigned short	usExtraFieldLength;
					unsigned short	usFileCommentLength;
					unsigned short	usDiskNumberStart;
					unsigned short	usInternalFileAttributes;
					unsigned int	uiExternalFileAttributes;
					unsigned int	uiOffsetOfLocalHeader;
				};

				struct ZipLocalFileHeader
				{
					unsigned int	uiSignature;
					unsigned short	usVersionToExtract;
					unsigned short	usBitFlag;
					unsigned short	usCompressionMethod;
					unsigned short	usLastModFileTime;
					unsigned short	usLastModFileDate;
					unsigned int	uiCRC32;
					unsigned int	uiCompressedSize;
					unsigned int	uiUncompressedSize;
					unsigned short	usFilenameLength;
					unsigned short	usExtraFieldLength;
				};

#ifdef WIN32
				#pragma pack(pop)
#else			
				// GCC doesn't support push/pop, assume 32bit system
				#pragma pack(4)
#endif

				std::vector <ZipDirectoryEntry> m_vZipDirectory;
			};
		}
	}
}
#endif
