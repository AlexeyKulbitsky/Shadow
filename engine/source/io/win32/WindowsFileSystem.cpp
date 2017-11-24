#include "WindowsFileSystem.h"

#include "../../video/TextureLoader/TextureLoader.h"
#include "../../Device.h"

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
		//String absolutePath = m_workingDirectoryPath + "/" + folder + "/";

		String absolutePath = folder + "/";

		String folderName = folder;
		String::size_type pos = String(folder).find_last_of("\\/");
		if (pos != String::npos)
			folderName = folder.substr(pos + 1);

		m_root.reset(new FolderInfo(folderName, absolutePath));

		CollectFilesFromFolder(absolutePath, m_root, true);

		UpdateResourceGroups();
		ResourceManager::GetInstance()->UpdateResourceGroups();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	void WindowsFileSystem::AddInternalFolder(const String& folder)
	{
		String absolutePath = m_workingDirectoryPath + "/" + folder + "/";

		String folderName = folder;
		String::size_type pos = String(folder).find_last_of("\\/");
		if (pos != String::npos)
			folderName = folder.substr(pos + 1);

		SPtr<FolderInfo> folderInfo(new FolderInfo(folderName, absolutePath));

		CollectFilesFromFolder(absolutePath, folderInfo, true);
		m_internalDataRoots.push_back(folderInfo);

		UpdateResourceGroups();
		ResourceManager::GetInstance()->UpdateResourceGroups();
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

	File WindowsFileSystem::LoadFile(const String& filename)
	{
		std::weak_ptr<FileSystemComponent> result;
		if (m_root)
		{
			result = m_root->FindChildByName(filename);
			if (result.expired())
			{
				for (auto internalRoot : m_internalDataRoots)
				{
					result = internalRoot->FindChildByName(filename);
					if (!result.expired())
						break;
				}
			}
		}
		else
		{
			for (auto internalRoot : m_internalDataRoots)
			{
				result = internalRoot->FindChildByName(filename);
				if (!result.expired())
					break;
			}
		}
		if (result.expired())
			return File();

		File file = LoadFile(std::static_pointer_cast<FileInfo>(result.lock()));
		return file;
	}

	File WindowsFileSystem::LoadFile(std::weak_ptr<FileInfo> fileInfo)
	{
		if (fileInfo.expired())
			return File();

		const auto path = fileInfo.lock()->absolutePath;

		std::ifstream file(path, std::ios::ate | std::ios::binary);

		SH_ASSERT(file.is_open(), "failed to open file!");
		if (!file.is_open())
			return File();

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		File result;
		result.m_data = std::move(buffer);
		return result;
	}

	bool WindowsFileSystem::CreateFolder(const String& path)
	{
		return static_cast<bool>(CreateDirectory(path.c_str(), NULL));
	}

	bool WindowsFileSystem::Rename(const String& oldName, const String& newName)
	{
		 return static_cast<bool>(MoveFile(oldName.c_str(), newName.c_str()));
	}

	bool WindowsFileSystem::Delete(const String& filePath)
	{
		return static_cast<bool>(DeleteFile(filePath.c_str()));
	}

	bool WindowsFileSystem::Copy(const String& srcFileName, const String& dstFileName)
	{
		return static_cast<bool>(CopyFile(srcFileName.c_str(), dstFileName.c_str(), TRUE));
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
					//FileInfo info;
					//info.name = FindFileData.cFileName;
					//info.absolutePath = originalPath + info.name;
					//m_fileList.insert(info);

					SPtr<FileInfo> fileInfo(new FileInfo(FindFileData.cFileName, 
						originalPath + FindFileData.cFileName));
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

} // io

} // sh

