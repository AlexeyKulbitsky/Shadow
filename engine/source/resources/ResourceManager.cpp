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

		return nullptr;
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
			m_defaultMaterial.reset(new video::Material());
			m_defaultMaterial->SetRenderTechnique("default.rt");
		}
		return m_defaultMaterial;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	const video::TexturePtr& ResourceManager::GetDefaultTexture()
	{
		return video::TextureLoader::GetInstance()->GetWhiteTexture();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

}