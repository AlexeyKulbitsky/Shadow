#include "WindowsFileSystem.h"

#include "../../video/TextureLoader/TextureLoader.h"

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
		m_root.reset(new FolderInfo(folder, absolutePath));

		CollectFilesFromFolder(absolutePath, m_root, true);

		m_folders.insert(folder);

		//UpdateFileGroups();
		UpdateResourceGroups();
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
		// First try to open file by direct path (local folder)
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			// Else try to find file in local filesystem and open it
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

	void WindowsFileSystem::CollectFilesFromFolder(const String& folder, const SPtr<FolderInfo>& root, bool recursive)
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

					SPtr<FileInfo> fileInfo(new FileInfo(FindFileData.cFileName, info.absolutePath));
					root->children.push_back(fileInfo);
				}
				else
				{
					if (strcmp(".", FindFileData.cFileName) &&
						strcmp("..", FindFileData.cFileName) &&
						recursive)
					{
						String chilfFolderAbsolutePath = originalPath + FindFileData.cFileName;

						SPtr<FolderInfo> folderInfo(new FolderInfo(FindFileData.cFileName, chilfFolderAbsolutePath + "/"));
						root->children.push_back(folderInfo);

						CollectFilesFromFolder(chilfFolderAbsolutePath, folderInfo, true);
					}
				}
			} while (FindNextFile(hFind, &FindFileData));
			FindClose(hFind);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	void WindowsFileSystem::UpdateFileGroups()
	{
		m_imageFileNames.clear();
		const auto& imageExtensions = video::TextureLoader::GetInstance()->GetAvalilableExtensions();

		for (const auto& file : m_fileList)
		{
			size_t pos = file.name.find_last_of('.');
			auto extension = file.name.substr(pos + 1);

			// Check for image
			auto res = std::find(imageExtensions.begin(), imageExtensions.end(), extension);
			if (res != imageExtensions.end())
				m_imageFileNames.push_back(file.name);

			// Check for material
			if (extension == "mat")
			{
				m_materialFileNames.push_back(file.name);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

} // io

} // sh

