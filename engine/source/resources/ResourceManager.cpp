#include "ResourceManager.h"
#include "../Device.h"
#include "../video/RenderTechnique.h"
#include "../video/Material.h"
#include "../video/Texture.h"
#include "../video/TextureLoader/TextureLoader.h"
#include "../scene/ModelBase.h"
#include "../scene/ModelLoader/ModelLoader.h"
#include "../scene/ModelLoader/AssimpModelLoader.h"
#include "scene/ModelLoader/TinyObjModelLoader.h"

#include <pugixml.hpp>

namespace sh
{
	ResourceManager::ResourceManager()
	{
		video::TextureLoader::CreateInstance();
				// Model loader
#if defined (SHADOW_ASSIMP_LOADER)
		scene::ModelLoader::CreateInstance<scene::AssimpModelLoader>();
#else
		scene::ModelLoader::CreateInstance<scene::TinyObjModelLoader>();
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	ResourceManager::~ResourceManager()
	{
		video::TextureLoader::DestroyInstance();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ResourceManager::Init()
	{
		
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ResourceManager::Unload()
	{
		for (size_t i = 0; i < m_renderTechniques.size(); ++i)
		{
			m_renderTechniques[i]->Unload();
		}
		for (size_t i = 0; i < m_textures.size(); ++i)
		{
			m_textures[i]->Unload();
		}
		for (size_t i = 0; i < m_models.size(); ++i)
		{
			m_models[i]->Unload();
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	video::TexturePtr ResourceManager::GetTexture(const String& fileName)
	{
		// Check if Texture already exists in textures pool
		for (size_t i = 0, sz = m_textures.size(); i < sz; ++i)
		{
			if (m_textures[i]->GetFileInfo().lock()->name == fileName)
				return m_textures[i];
		}

		video::TexturePtr texture = video::TextureLoader::GetInstance()->Load(fileName);
		if (texture)
		{
			//texture->SetFileName(fileName);
			io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
			const auto& textureFileInfos = fs->GetImageFileInfos();
			for (const auto& textureFileInfo : textureFileInfos)
			{
				if (!textureFileInfo.expired() && textureFileInfo.lock()->name == fileName)
				{
					texture->SetFileInfo(textureFileInfo);
				}
			}

			m_textures.push_back(texture);
			return texture;
		}

		return nullptr;
	}

	video::TexturePtr ResourceManager::GetCubeTexture(const std::vector<String>& faces)
	{
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		
		video::TexturePtr texture = video::TextureLoader::GetInstance()->LoadCube(faces);
		
		return texture;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	video::RenderTechniquePtr ResourceManager::GetRenderTechnique(const String& fileName)
	{
		// Check if RenderTechnique already exists in techniques pool
		for (size_t i = 0, sz = m_renderTechniques.size(); i < sz; ++i)
		{
			if (m_renderTechniques[i]->GetFileInfo().lock()->name == fileName)
				return m_renderTechniques[i];
		}
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		io::File file = fs->LoadFile(fileName);

		video::RenderTechniquePtr rt(new video::RenderTechnique());
		rt->Load(file.GetData());
		const auto& rtFileInfos = fs->GetRenderTechniqueFileInfos();
		for (const auto& rtFileInfo : rtFileInfos)
		{
			if (!rtFileInfo.expired() && rtFileInfo.lock()->name == fileName)
			{
				rt->SetFileInfo(rtFileInfo);
			}
		}
		//rt->SetFileName(fileName);
		m_renderTechniques.push_back(rt);
		return rt;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ResourceManager::AddMaterial(const video::MaterialPtr& material)
	{
		for (size_t i = 0U, sz = m_materials.size(); i < sz; ++i)
		{
			if (m_materials[i]->GetFileInfo().lock()->name == material->GetFileInfo().lock()->name)
			{
				return;
			}
		}
		m_materials.push_back(material);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	video::MaterialPtr ResourceManager::GetMaterial(const String& materialName)
	{
		for (size_t i = 0U, sz = m_materials.size(); i < sz; ++i)
		{
			if (m_materials[i]->GetFileInfo().lock()->name == materialName)
			{
				return m_materials[i];
			}
		}

		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		io::File file = fs->LoadFile(materialName);

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_buffer(file.GetData().data(), file.GetData().size());
			
		if (result.status != pugi::status_ok)
			return video::MaterialPtr();

		// Read material
		pugi::xml_node materialNode = doc.child("material");
		while (materialNode)
		{
			video::MaterialPtr material(new video::Material());
			material->Load(materialNode);
			//material->SetFileName(materialName);
			m_materials.push_back(material);

			const auto& materialFileInfos = fs->GetMaterialFileInfos();
			for (const auto& materialFileInfo : materialFileInfos)
			{
				if (!materialFileInfo.expired() && materialFileInfo.lock()->name == materialName)
				{
					material->SetFileInfo(materialFileInfo);
				}
			}

			return material;
		}

		return video::MaterialPtr();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	scene::ModelBasePtr ResourceManager::GetModelBase(const String& fileName)
	{
		// Check if Model already exists in models pool
		for (size_t i = 0, sz = m_models.size(); i < sz; ++i)
		{
			if (m_models[i]->GetFileInfo().lock()->name == fileName)
				return m_models[i];
		}

		scene::ModelBasePtr model = scene::ModelLoader::GetInstance()->Load(fileName);
		if (model)
		{
			//model->SetFileName(fileName);

			io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
			const auto& modelFileInfos = fs->GetModelFileInfos();
			for (const auto& modelFileInfo : modelFileInfos)
			{
				if (!modelFileInfo.expired() && modelFileInfo.lock()->name == fileName)
				{
					model->SetFileInfo(modelFileInfo);
				}
			}
			m_models.push_back(model);
			return model;
		}

		return nullptr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	const video::MaterialPtr& ResourceManager::GetDefaultMaterial()
	{
		if (!m_defaultMaterial)
		{
			io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
			io::File file = fs->LoadFile("default.mat");

			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_buffer(file.GetData().data(), file.GetData().size());

			// Read material
			pugi::xml_node materialNode = doc.child("material");
			if (materialNode)
			{
				m_defaultMaterial.reset(new video::Material());
				m_defaultMaterial->Load(materialNode);

				const auto& materialFileInfos = fs->GetMaterialFileInfos();
				for (const auto& materialFileInfo : materialFileInfos)
				{
					if (!materialFileInfo.expired() && materialFileInfo.lock()->name == "default.mat")
					{
						m_defaultMaterial->SetFileInfo(materialFileInfo);
					}
				}
			}
		}
		return m_defaultMaterial;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	const video::TexturePtr& ResourceManager::GetDefaultTexture()
	{
		return video::TextureLoader::GetInstance()->GetCheckerTexture();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void ResourceManager::UpdateResourceGroups()
	{
		auto fileSystem = io::FileSystem::GetInstance();
		const auto& imageExtensions = video::TextureLoader::GetInstance()->GetAvalilableExtensions();
		const auto& modelExtensions = scene::ModelLoader::GetInstance()->GetAvalilableExtensions();

		m_resourceGroups.clear();

		// Update project assets folder
		auto rootFolder = fileSystem->GetRoot();
		if (rootFolder)
		{
			UpdateRecursive(rootFolder, imageExtensions, modelExtensions);
		}

		const auto& internalFolders = fileSystem->GetInternalFolders();
		for (const auto& internalFolder : internalFolders)
		{
			UpdateRecursive(internalFolder, imageExtensions, modelExtensions);
		}

	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	ResourceRefList ResourceManager::GetResourceGroup(const String& name)
	{
		auto it = m_resourceGroups.find(name);
		if (it == m_resourceGroups.end())
			return ResourceRefList();

		return it->second;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ResourceManager::UpdateRecursive(const SPtr<io::FolderInfo>& folder,
		const std::vector<String>& imageExtensions,
		const std::vector<String>& modelExtensions)
	{
		for (const auto& child : folder->children)
		{
			switch (child->GetType())
			{
			case io::FileSystemComponent::Type::File:
			{
				size_t pos = child->name.find_last_of('.');
				auto extension = child->name.substr(pos + 1);

				// Check for image
				auto resImage = std::find(imageExtensions.begin(), imageExtensions.end(), extension);
				if (resImage != imageExtensions.end())
				{
					m_resourceGroups["Textures"].push_back(ResourceRef("Textures", child->name));
				}

				// Check for model
				auto resModel = std::find(modelExtensions.begin(), modelExtensions.end(), extension);
				if (resModel != modelExtensions.end())
				{
					m_resourceGroups["Models"].push_back(ResourceRef("Models", child->name));
				}

				// Check for material
				if (extension == "mat")
				{
					m_resourceGroups["Materials"].push_back(ResourceRef("Materials", child->name));
				}

				// Check for render technique
				if (extension == "rt")
				{
					m_resourceGroups["RenderTechniques"].push_back(ResourceRef("RenderTechniques", child->name));
				}
			}
			break;
			case io::FileSystemComponent::Type::Folder:
			{
				SPtr<io::FolderInfo> childFolderInfo = std::static_pointer_cast<io::FolderInfo>(child);
				UpdateRecursive(childFolderInfo, imageExtensions, modelExtensions);
			}
			break;
			default:
				break;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
}