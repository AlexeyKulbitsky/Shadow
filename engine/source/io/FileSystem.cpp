#include "FileSystem.h"

#include "../video/TextureLoader/TextureLoader.h"
#include "../scene/ModelLoader/ModelLoader.h"

namespace sh
{
	namespace io
	{

		void FileSystemComponent::Rename(const String& newName)
		{
			auto oldFileName = absolutePath;
			size_t separatorPos = oldFileName.find_last_of('/');
			auto newFilename = oldFileName.substr(0U, separatorPos) + "/" + newName;
			FileSystem::GetInstance()->Rename(oldFileName, newFilename);

			absolutePath = newFilename;
			name = newName;
		}

		std::weak_ptr<FileSystemComponent> FolderInfo::FindChildByName(const String& name)
		{
			for (const auto& child : children)
			{
				if (child->name == name)
					return child;
				if (child->GetType() == FileSystemComponent::Type::Folder)
				{
					auto folderInfo = std::static_pointer_cast<FolderInfo>(child);
					auto childInfo = folderInfo->FindChildByName(name);
					if (!childInfo.expired())
						return childInfo;
				}
			}
			return SPtr<FileSystemComponent>();
		}

		FileSystem::FileSystem()
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		FileSystem::~FileSystem()
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		void FileSystem::UpdateResourceGroups()
		{
			m_imageFileInfos.clear();
			m_modelFileInfos.clear();
			m_materialFileInfos.clear();
			m_renderTechniqueFileInfos.clear();
			const auto& imageExtensions = video::TextureLoader::GetInstance()->GetAvalilableExtensions();
			const auto& modelExtensions = scene::ModelLoader::GetInstance()->GetAvalilableExtensions();
			
			// Update project assets folder
			if (m_root)
			{
				UpdateRecursive(m_root, imageExtensions, modelExtensions);
			}

			for (const auto& internalFolder : m_internalDataRoots)
			{
				UpdateRecursive(internalFolder, imageExtensions, modelExtensions);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		void FileSystem::UpdateRecursive(const SPtr<FolderInfo>& folder, 
										 const std::vector<String>& imageExtensions,
										 const std::vector<String>& modelExtensions)
		{
			for (const auto& child : folder->children)
			{
				switch (child->GetType())
				{
				case FileSystemComponent::Type::File:
				{
					size_t pos = child->name.find_last_of('.');
					auto extension = child->name.substr(pos + 1);

					// Check for image
					auto resImage = std::find(imageExtensions.begin(), imageExtensions.end(), extension);
					if (resImage != imageExtensions.end())
					{
						m_imageFileInfos.push_back(std::static_pointer_cast<FileInfo>(child));
					}

					// Check for model
					auto resModel = std::find(modelExtensions.begin(), modelExtensions.end(), extension);
					if (resModel != modelExtensions.end())
					{
						m_modelFileInfos.push_back(std::static_pointer_cast<FileInfo>(child));
					}

					// Check for material
					if (extension == "mat")
					{
						m_materialFileInfos.push_back(std::static_pointer_cast<FileInfo>(child));
					}

					// Check for render technique
					if (extension == "rt")
					{
						m_renderTechniqueFileInfos.push_back(std::static_pointer_cast<FileInfo>(child));
					}
				}
				break;
				case FileSystemComponent::Type::Folder:
				{
					SPtr<FolderInfo> childFolderInfo = std::static_pointer_cast<FolderInfo>(child);
					UpdateRecursive(childFolderInfo, imageExtensions, modelExtensions);
				}
				break;
				default:
					break;
				}
			}
		}
	}
}