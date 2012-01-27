#include "ArchiveManager.h"

#include <algorithm>

using std::string;
using std::replace;

namespace System
{
	namespace FileSystem
	{
		///////////////////////////////////////////////////////////////////////
		bool nocase_compare (char c1, char c2)
		///////////////////////////////////////////////////////////////////////
		{
			return toupper(c1) == toupper(c2);
		}


		///////////////////////////////////////////////////////////////////////
		ArchiveManager::ArchiveManager(void)
		///////////////////////////////////////////////////////////////////////
		{
		}

		///////////////////////////////////////////////////////////////////////
		ArchiveManager::~ArchiveManager(void)
		///////////////////////////////////////////////////////////////////////
		{
			RemoveAllArchives ();
		}

		///////////////////////////////////////////////////////////////////////
		void ArchiveManager::AddArchive (PArchive pArchive)
		///////////////////////////////////////////////////////////////////////
		{
			m_vArchiveList.push_back (pArchive);
		}

		///////////////////////////////////////////////////////////////////////
		void ArchiveManager::RemoveArchive (PArchive pArchive)
		///////////////////////////////////////////////////////////////////////
		{
			for (ArchiveList::iterator i = m_vArchiveList.begin();
				i != m_vArchiveList.end (); i++)
			{
				if ((*i) == pArchive)
					m_vArchiveList.erase (i);
			}
		}

		///////////////////////////////////////////////////////////////////////
		void ArchiveManager::RemoveAllArchives ()
		///////////////////////////////////////////////////////////////////////
		{
			m_vArchiveList.clear ();
		}

		///////////////////////////////////////////////////////////////////////
		bool ArchiveManager::FileExists (std::string filename)
		///////////////////////////////////////////////////////////////////////
		{
			string relativeFile = "";

			// Convert to unix path
			filename = ConvertPath (filename);

			for (ArchiveList::iterator i = m_vArchiveList.begin();
				i != m_vArchiveList.end (); i++)
			{
				relativeFile = GetRelativePath (filename, *i);

				// If the paths match, force an exists check
				if (relativeFile != "")
					return (*i)->FileExists (relativeFile);

				// Check if it's a relative path to a file
				if ((*i)->FileExists (filename))
					return true;
			}	

			return false;
		}

		///////////////////////////////////////////////////////////////////////
		PIStream ArchiveManager::LoadFile (string filename)
		///////////////////////////////////////////////////////////////////////
		{
			string relativeFile = "";

			// Convert to unix path
			filename = ConvertPath (filename);

			for (ArchiveList::iterator i = m_vArchiveList.begin();
				i != m_vArchiveList.end (); i++)
			{
				relativeFile = GetRelativePath (filename, *i);

				// If the paths match, force an exists check
				if (relativeFile != "")
					return (*i)->LoadFile (relativeFile);

				// Check if it's a relative path to a file
				if ((*i)->FileExists (filename))
					return (*i)->LoadFile (filename);
			}	

			return NULL;
		}

		///////////////////////////////////////////////////////////////////////
		POStream ArchiveManager::WriteFile (string filename)
		///////////////////////////////////////////////////////////////////////
		{
			string relativeFile = "";

			// Convert to unix path
			filename = ConvertPath (filename);

			// First check for an absolute path
			for (ArchiveList::iterator i = m_vArchiveList.begin();
				i != m_vArchiveList.end (); i++)
			{
				relativeFile = GetRelativePath (filename, *i);

				// If the paths match, force an exists check
				if (relativeFile != "")
				{
					// Make sure the archive is writable
					if (!(*i)->Writable ())
						return NULL;

					return (*i)->WriteFile (relativeFile);
				}
			}	

			// Write to the first writable archive
			for (ArchiveList::iterator i = m_vArchiveList.begin();
				i != m_vArchiveList.end (); i++)
			{
				if ((*i)->Writable ())
					return (*i)->WriteFile (filename);
			}

			return NULL;
		}

		///////////////////////////////////////////////////////////////////////
		string ArchiveManager::GetRelativePath (string fullPath, PArchive archive)
		///////////////////////////////////////////////////////////////////////
		{
			// Early failure
			if (fullPath.length () <= archive->GetArchiveName().length())
				return "";
			
			// Full paths don't match
			string path = fullPath.substr (0, archive->GetArchiveName ().length());
			if (!equal (path.begin(), path.end(), archive->GetArchiveName().begin(),
				nocase_compare))
				return "";

			// Return the relative path
			return fullPath.substr (archive->GetArchiveName ().length());
		}

		///////////////////////////////////////////////////////////////////////
		string ArchiveManager::ConvertPath (string path)
		///////////////////////////////////////////////////////////////////////
		{
			// Replace all the backslashes with forward slashes
			std::replace (path.begin(), path.end(), '\\', '/');
			return path;
		}
	}
}
