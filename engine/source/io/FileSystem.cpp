#include "FileSystem.h"
#include <windows.h>

namespace sh
{
	namespace io
	{
		FileSystem::FileSystem()
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		FileSystem::~FileSystem()
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		void FileSystem::Init()
		{
			// Get working directory path
			char buffer[MAX_PATH];
			GetModuleFileName(NULL, buffer, MAX_PATH);
			String::size_type pos = String(buffer).find_last_of("\\/");
			m_workingDirectoryPath = String(buffer).substr(0, pos) + "/";

			//printf("Working dir: %s\n", m_workingDirectoryPath.c_str());

			for (auto it : m_folders)
			{
				String absolutePath = m_workingDirectoryPath + "/" + it + "/";
				CollectFilesFromFolder(absolutePath, true);
			}

			for (auto it : m_fileList)
			{			
				//printf("File: %s\n", it.name.c_str());
				//printf("Path: %s\n", it.absolutePath.c_str());
				//printf("------------------------------------\n");
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		const String& FileSystem::GetWorkingDirectory() const
		{
			return m_workingDirectoryPath;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		void FileSystem::AddFolder(const String& folder, bool recursive)
		{
			m_folders.insert(folder);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		const FileInfo& FileSystem::FindFile(const String& fileName)
		{
			static const FileInfo errorFile("", "");
			HashSet<FileInfo>::iterator it = m_fileList.find(FileInfo(fileName, ""));
			if (it != m_fileList.end())
			{
				return *it;
			}
			return errorFile;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		void FileSystem::CollectFilesFromFolder(const String& folder, bool recursive)
		{
			
			WIN32_FIND_DATA FindFileData;
			HANDLE hFind;

			String originalPath = folder + "/";
			String searchFilter = originalPath + "*";
			hFind = FindFirstFile(searchFilter.c_str(), &FindFileData);

			if (hFind != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						FileInfo info;
						info.name = FindFileData.cFileName;
						info.absolutePath = originalPath + info.name;
						m_fileList.insert(info);
					}
					else
					{
						if (strcmp(".", FindFileData.cFileName) && 
							strcmp("..", FindFileData.cFileName) &&
							recursive)
						{
							String chilfFolderAbsolutePath = originalPath + FindFileData.cFileName;
							CollectFilesFromFolder(chilfFolderAbsolutePath, true);
						}
					}
				} 
				while (FindNextFile(hFind, &FindFileData));
				FindClose(hFind);
			}
			
		}

	}
}