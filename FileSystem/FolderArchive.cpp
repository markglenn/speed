#include <sys/stat.h>

#include "FolderArchive.h"
#include "ArchiveManager.h"

using namespace std;

#ifdef WIN32
#define statstruct __stat64
#define statfunc _stat64
#else
#define statstruct stat
#define statfunc stat
#endif

namespace System
{
	namespace FileSystem
	{
		///////////////////////////////////////////////////////////////////////
		FolderArchive::FolderArchive(string szFolderName)
		///////////////////////////////////////////////////////////////////////
		{
			// Dos to unix filename conversion
			m_szFolderName = ArchiveManager::ConvertPath (szFolderName);

			// Add in the last slash
			if (m_szFolderName[m_szFolderName.length() - 1] != '/')
				m_szFolderName.push_back ('/');
		}

		///////////////////////////////////////////////////////////////////////
		FolderArchive::~FolderArchive(void)
		///////////////////////////////////////////////////////////////////////
		{
		}

		///////////////////////////////////////////////////////////////////////
		bool FolderArchive::FileExists (string filename)
		///////////////////////////////////////////////////////////////////////
		{
			struct statstruct buf;

			if (!statfunc ( (m_szFolderName + filename).c_str(), &buf ) )
			return true;

			return false;
		}

		///////////////////////////////////////////////////////////////////////
		PIStream FolderArchive::LoadFile (string filename)
		///////////////////////////////////////////////////////////////////////
		{
			ifstream *pStream = new ifstream ((m_szFolderName + filename).c_str());

			// If the file isn't open, return null
			if (!pStream->is_open())
			{
				delete pStream;
				return NULL;
			}

			// Return the stream pointer
			return pStream;
		}

		///////////////////////////////////////////////////////////////////////
		POStream FolderArchive::WriteFile (string filename)
		///////////////////////////////////////////////////////////////////////
		{
			ofstream *pStream = new ofstream ((m_szFolderName + filename).c_str());

			if (!pStream->is_open())
			{
				delete pStream;
				return NULL;
			}

			// Return the stream pointer
			return pStream;
		}
	}

}
