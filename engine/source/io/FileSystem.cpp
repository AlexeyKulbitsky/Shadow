#include "FileSystem.h"

#include "../video/TextureLoader/TextureLoader.h"

namespace sh
{
	namespace io
	{

		void FileSystemComponent::Rename(const String& newName)
		{
			FileSystem::GetInstance()->Rename(this, newName);
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
			m_materialFileInfos.clear();
			m_renderTechniqueFileInfos.clear();
			const auto& imageExtensions = video::TextureLoader::GetInstance()->GetAvalilableExtensions();
			
			UpdateRecursive(m_root, imageExtensions);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		void FileSystem::UpdateRecursive(const SPtr<FolderInfo>& folder, const std::vector<String>& imageExtensions)
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
					auto res = std::find(imageExtensions.begin(), imageExtensions.end(), extension);
					if (res != imageExtensions.end())
					{
						m_imageFileInfos.push_back(std::static_pointer_cast<FileInfo>(child));
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
					UpdateRecursive(childFolderInfo, imageExtensions);
				}
				break;
				default:
					break;
				}
			}
		}
	}
}