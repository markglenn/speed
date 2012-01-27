#include <iostream>

#include "../ArchiveManager.h"
#include "ZipArchive.h"
#include "basic_zip_istream.h"
#include "../stored_streambuf.h"
#include "../generic_istream.h"

using namespace std;

namespace System
{
	namespace FileSystem
	{
		namespace Zip
		{
			///////////////////////////////////////////////////////////////////////
			ZipArchive::ZipArchive(string szZipName)
			///////////////////////////////////////////////////////////////////////
			{
				// Dos to unix filename conversion
				m_szZipFile = ArchiveManager::ConvertPath (szZipName);

				Open (m_szZipFile);
			}

			///////////////////////////////////////////////////////////////////////
			ZipArchive::~ZipArchive(void)
			///////////////////////////////////////////////////////////////////////
			{
			}

			///////////////////////////////////////////////////////////////////////
			bool ZipArchive::Open (string zipFile)
			///////////////////////////////////////////////////////////////////////
			{
				ZipLocalFileHeader	oFileHeader;
				ZipDirectoryEntry	oDirEntry;
				char				szFilename[1024];
				ifstream			fZipFile;

				// Try to open the zip file
				fZipFile.open (zipFile.c_str(), ios::in | ios::binary);

				// If the file didn't open, return false
				if (!fZipFile.is_open ())
					return false;

				while (true)
				{
					fZipFile.read ((char *)&oFileHeader, sizeof(ZipLocalFileHeader));

					// If the next block is a central directory, exit
					if (oFileHeader.uiSignature == ZIP_DIRECTORY_SIGNATURE)
						return true;

					// File does not match PKZIP format
					if (oFileHeader.uiSignature != ZIP_SIGNATURE)
						return false;

					// Read in the filename
					fZipFile.read (szFilename, oFileHeader.usFilenameLength);
					szFilename[oFileHeader.usFilenameLength] = 0;

					// Skip the extra field
					fZipFile.seekg (oFileHeader.usExtraFieldLength, ios::cur);

					// Set the directory entry
					oDirEntry.szFilename	= szFilename;
					oDirEntry.eCompression	= (ECompressionMethod)oFileHeader.usCompressionMethod;
					oDirEntry.ulOffset		= fZipFile.tellg ();
					oDirEntry.ulFileLength	= oFileHeader.uiCompressedSize;

					// Add the directory entry to the directory list
					m_vZipDirectory.push_back (oDirEntry);

					// Skip the compressed data
					fZipFile.seekg (oFileHeader.uiCompressedSize, ios::cur);
				}
			}

			///////////////////////////////////////////////////////////////////////
			PIStream ZipArchive::LoadFile (string filename) 
			///////////////////////////////////////////////////////////////////////
			{
				std::vector <ZipDirectoryEntry>::iterator i;

				for (i = m_vZipDirectory.begin(); i != m_vZipDirectory.end(); i++)
				{
					if (filename == (*i).szFilename)
					{
						// Unknown compression?
						if ((*i).eCompression != ZIP_COMPRESSION_DEFLATE && 
							(*i).eCompression != ZIP_COMPRESSION_NONE)
							return NULL;

						// Return the stream
						streambuf *pStreamBuf;

						if ((*i).eCompression == ZIP_COMPRESSION_DEFLATE)
							pStreamBuf = new zip_streambuf (GetArchiveName (), (*i).ulOffset,
								(*i).ulFileLength);
						else
							pStreamBuf = new stored_streambuf (GetArchiveName (), (*i).ulOffset,
								(*i).ulFileLength);

						return new generic_istream (pStreamBuf);
					}
				}

				return NULL;
			}

			///////////////////////////////////////////////////////////////////////
			bool ZipArchive::FileExists (string filename)
			///////////////////////////////////////////////////////////////////////
			{
				std::vector <ZipDirectoryEntry>::iterator i;

				for (i = m_vZipDirectory.begin(); i != m_vZipDirectory.end(); i++)
				{
					if (filename == (*i).szFilename)
						return true;
				}

				return false;
			}
		}
	}
}
