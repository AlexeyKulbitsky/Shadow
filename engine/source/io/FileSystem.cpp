#include "FileSystem.h"

#include "../video/TextureLoader/TextureLoader.h"

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

		void FileSystem::UpdateResourceGroups()
		{
			m_imageFileNames.clear();
			m_materialFileNames.clear();
			m_renderTechniqueFileNames.clear();
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
						m_imageFileNames.push_back(child->name);

					// Check for material
					if (extension == "mat")
					{
						m_materialFileNames.push_back(child->name);
					}

					// Check for render technique
					if (extension == "rt")
					{
						m_renderTechniqueFileNames.push_back(child->name);
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