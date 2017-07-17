#if defined (SHADOW_WINDOWS)
#include "WindowsFileSystem.h"

#include <windows.h>
#include <fstream>

namespace sh
{

namespace io
{

	WindowsFileSystem::WindowsFileSystem()
	{
		// Get working directory path
		char buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		String::size_type pos = String(buffer).find_last_of("\\/");
		m_workingDirectoryPath = String(buffer).substr(0, pos) + "/";
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	WindowsFileSystem::~WindowsFileSystem()
	{

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	void WindowsFileSystem::Init()
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	const String& WindowsFileSystem::GetWorkingDirectory() const
	{
		return m_workingDirectoryPath;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	void WindowsFileSystem::AddFolder(const String& folder, bool recursive)
	{
		if (m_folders.find(folder) != m_folders.end())
			return;

		String absolutePath = m_workingDirectoryPath + "/" + folder + "/";
		CollectFilesFromFolder(absolutePath, true);

		m_folders.insert(folder);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	const FileInfo& WindowsFileSystem::FindFile(const String& fileName)
	{
		static const FileInfo errorFile("", "");
		Set<FileInfo>::iterator it = m_fileList.find(FileInfo(fileName, ""));
		if (it != m_fileList.end())
		{
			return *it;
		}
		return errorFile;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	bool WindowsFileSystem::SaveFile(const std::vector<char>& data, const String& fileName)
	{
		std::ofstream f(m_workingDirectoryPath + "/" + fileName, std::ofstream::out);
		f.write(data.data(), data.size());
		f.close();
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	bool WindowsFileSystem::SaveFile(const String& data, const String& fileName)
	{
		std::ofstream f(m_workingDirectoryPath + "/" + fileName, std::ofstream::out);
		f << data;
		f.close();
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<char> WindowsFileSystem::ReadFile(const String& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			const auto& info = FindFile(filename);
			file = std::ifstream(info.absolutePath, std::ios::ate | std::ios::binary);
		}

		SH_ASSERT(file.is_open(), "failed to open file!");

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	File WindowsFileSystem::LoadFile(const String& filename)
	{
		File file;
		file.m_data = ReadFile(filename);
		return file;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	void WindowsFileSystem::CollectFilesFromFolder(const String& folder, bool recursive)
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
			} while (FindNextFile(hFind, &FindFileData));
			FindClose(hFind);
		}

	}

} // io

} // sh

#endif