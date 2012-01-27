#include <iostream>

#include "TarArchive.h"
#include "../ArchiveManager.h"

#include "../generic_istream.h"
#include "../stored_streambuf.h"

using namespace std;

/* The magic field is filled with this if uname and gname are valid. */
#define    TMAGIC    "ustar"        /* 7 chars and a null */

/* The magic field is filled with this if this is a GNU format dump entry */
#define    GNUMAGIC  "GNUtar"        /* 7 chars and a null */

/* The linkflag defines the type of file */
#define  LF_OLDNORMAL '\0'       /* Normal disk file, Unix compatible */
#define  LF_NORMAL    '0'        /* Normal disk file */
#define  LF_LINK      '1'        /* Link to previously dumped file */
#define  LF_SYMLINK   '2'        /* Symbolic link */
#define  LF_CHR       '3'        /* Character special file */
#define  LF_BLK       '4'        /* Block special file */
#define  LF_DIR       '5'        /* Directory */
#define  LF_FIFO      '6'        /* FIFO special file */
#define  LF_CONTIG    '7'        /* Contiguous file */

/* Further link types may be defined later. */

/* Bits used in the mode field - values in octal */
#define  TSUID    04000        /* Set UID on execution */
#define  TSGID    02000        /* Set GID on execution */
#define  TSVTX    01000        /* Save text (sticky bit) */

/* File permissions */
#define  TUREAD   00400        /* read by owner */
#define  TUWRITE  00200        /* write by owner */
#define  TUEXEC   00100        /* execute/search by owner */
#define  TGREAD   00040        /* read by group */
#define  TGWRITE  00020        /* write by group */
#define  TGEXEC   00010        /* execute/search by group */
#define  TOREAD   00004        /* read by other */
#define  TOWRITE  00002        /* write by other */
#define  TOEXEC   00001        /* execute/search by other */

namespace System
{
	namespace FileSystem
	{
		namespace Tar
		{


			///////////////////////////////////////////////////////////////////////
			TarArchive::TarArchive(string tarFile)
			///////////////////////////////////////////////////////////////////////
			{
				m_szTarFilename = ArchiveManager::ConvertPath (tarFile);

				Open (m_szTarFilename);
			}

			///////////////////////////////////////////////////////////////////////
			TarArchive::~TarArchive(void)
			///////////////////////////////////////////////////////////////////////
			{
			}

			///////////////////////////////////////////////////////////////////////
			bool TarArchive::Open (std::string tarFile)
			///////////////////////////////////////////////////////////////////////
			{
				ifstream fTarFile;
				TarRecord record;
				TarDirectoryEntry dirEntry;

				// Try to open the zip file
				fTarFile.open (tarFile.c_str(), ios::in | ios::binary);

				// If the file didn't open, return false
				if (!fTarFile.is_open ())
					return false;

				do
				{
					fTarFile.read ((char *)&record, sizeof(TarRecord));

					// Null record, done reading
					if (strlen(record.name) == 0)
					{
						fTarFile.close ();
						return true;
					}

					// Check if this is a valid record
					if (strcmp (record.magic, TMAGIC) != 0 && strcmp (record.magic, GNUMAGIC) != 0)
					{
						fTarFile.close();
						return false;
					}

					// Setup the entry
					dirEntry.szFilename		= record.name;
					dirEntry.ulFileLength	= strtol (record.size, NULL, 8);
					dirEntry.ulOffset		= fTarFile.tellg ();
			        
					int nextFilePos = dirEntry.ulFileLength;
					
					if (dirEntry.szFilename != "")
					{
						// Align to 512 bytes
						if (nextFilePos % 512 != 0)
							nextFilePos += 512 - (nextFilePos % 512);

						// Seek to the next file
						fTarFile.seekg (nextFilePos, ios::cur);

						m_vTarDirectory.push_back (dirEntry);
					}
				} while (!fTarFile.eof());

				fTarFile.close ();

				return true;
			}

			///////////////////////////////////////////////////////////////////////
			PIStream TarArchive::LoadFile (std::string filename)
			///////////////////////////////////////////////////////////////////////
			{
				std::vector <TarDirectoryEntry>::iterator i;

				for (i = m_vTarDirectory.begin(); i != m_vTarDirectory.end(); i++)
				{
					if (filename == (*i).szFilename)
					{
						// Return the stream
						streambuf *pStreamBuf = new stored_streambuf (GetArchiveName (), (*i).ulOffset,
								(*i).ulFileLength);

						return new generic_istream (pStreamBuf);
					}
				}

				return NULL;
			}

			///////////////////////////////////////////////////////////////////////
			bool TarArchive::FileExists (std::string filename)
			///////////////////////////////////////////////////////////////////////
			{
				std::vector <TarDirectoryEntry>::iterator i;

				for (i = m_vTarDirectory.begin(); i != m_vTarDirectory.end(); i++)
				{
					if (filename == (*i).szFilename)
						return true;
				}

				return false;
			}
		}
	}
}