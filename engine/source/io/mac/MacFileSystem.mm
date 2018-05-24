#include "MacFileSystem.h"
#include "../../Device.h"
#include <fstream>
#include <mach-o/dyld.h>
#include <sys/stat.h>
#include <dirent.h>

//#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

namespace sh
{

namespace io
{

	MacFileSystem::MacFileSystem()
	{
		// Get working directory path
        char buffer[1024];
        uint32_t size = 1024;
        _NSGetExecutablePath(buffer, &size);
        String::size_type pos = String(buffer).find_last_of("\\/");
        m_workingDirectoryPath = String(buffer).substr(0, pos) + "/../../..";
        
        int a = 0;
        a++;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	MacFileSystem::~MacFileSystem()
	{
        
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	void MacFileSystem::Init()
	{
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	const String& MacFileSystem::GetWorkingDirectory() const
	{
		return m_workingDirectoryPath;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	void MacFileSystem::AddFolder(const String& folder, bool recursive)
	{
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

	void MacFileSystem::AddInternalFolder(const String& folder)
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

	bool MacFileSystem::SaveFile(const std::vector<char>& data, const String& fileName)
	{
		std::ofstream f(m_workingDirectoryPath + "/" + fileName, std::ofstream::out);
		f.write(data.data(), data.size());
		f.close();
		return true;

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	bool MacFileSystem::SaveFile(const String& data, const String& fileName)
	{
		std::ofstream f(m_workingDirectoryPath + "/" + fileName, std::ofstream::out);
		f << data;
		f.close();
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	File MacFileSystem::LoadFile(const String& filename)
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

	File MacFileSystem::LoadFile(std::weak_ptr<FileInfo> fileInfo)
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

	bool MacFileSystem::CreateFolder(const String& path)
	{
        NSFileManager *fileManager= [NSFileManager defaultManager];
        NSError *error = nil;
        NSString* str = [NSString stringWithCString:path.c_str() encoding:[NSString defaultCStringEncoding]];
        return [fileManager createDirectoryAtPath:str withIntermediateDirectories:YES attributes:nil error:&error];
	}

	bool MacFileSystem::Rename(const String& oldName, const String& newName)
	{
        NSFileManager *fileManager= [NSFileManager defaultManager];
        NSError *error = nil;
        NSString* oldPath = [NSString stringWithCString:oldName.c_str() encoding:[NSString defaultCStringEncoding]];
        NSString* newPath = [NSString stringWithCString:newName.c_str() encoding:[NSString defaultCStringEncoding]];
        return [fileManager moveItemAtPath:oldPath toPath:newPath error:&error];
    }

	bool MacFileSystem::Delete(const String& filePath)
	{
        NSFileManager *fileManager= [NSFileManager defaultManager];
        NSError *error = nil;
        NSString* str = [NSString stringWithCString:filePath.c_str() encoding:[NSString defaultCStringEncoding]];
        return [fileManager removeItemAtPath:str error:&error];
    }

	bool MacFileSystem::Copy(const String& srcFileName, const String& dstFileName)
	{
        NSFileManager *fileManager= [NSFileManager defaultManager];
        NSError *error = nil;
        NSString* srcPath = [NSString stringWithCString:srcFileName.c_str() encoding:[NSString defaultCStringEncoding]];
        NSString* dstPath = [NSString stringWithCString:dstFileName.c_str() encoding:[NSString defaultCStringEncoding]];
        return [fileManager copyItemAtPath:srcPath toPath:dstPath error:&error];
    }

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	void MacFileSystem::CollectFilesFromFolder(const String& folder, const SPtr<FolderInfo>& root, bool recursive)
	{
        DIR *dir;
        dirent *ent;
        struct stat st;
        
        dir = opendir(folder.c_str());
        while ((ent = readdir(dir)) != NULL)
        {
            const String file_name = ent->d_name;
            const String full_file_name = folder + "/" + file_name;
            
            if (file_name[0] == '.')
                continue;
            
            if (stat(full_file_name.c_str(), &st) == -1)
                continue;
            
            const bool is_directory = (st.st_mode & S_IFDIR) != 0;
            
            if (is_directory)
            {
                String chilfFolderAbsolutePath = folder + file_name;
                SPtr<FolderInfo> folderInfo(new FolderInfo(file_name, chilfFolderAbsolutePath + "/"));
                root->children.push_back(folderInfo);
                
                if (recursive)
                {
                    CollectFilesFromFolder(full_file_name, folderInfo, recursive);
                }
            }
            else
            {
                SPtr<FileInfo> fileInfo(new FileInfo(file_name, full_file_name));
                root->children.push_back(fileInfo);
            }
            
        }
        closedir(dir);
	}
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    void MacFileSystem::ShowOpenFileDialog()
    {
        // Open dialog
//        NSOpenPanel* openDlg = [NSOpenPanel openPanel];
//        [openDlg setPrompt:@"Select"];
//        
//        [openDlg beginWithCompletionHandler:^(NSInteger result){
//         
//         }];
    }

	///////////////////////////////////////////////////////////////////////////////////////////////////////

} // io

} // sh

