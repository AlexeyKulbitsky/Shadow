#include "ResourceManager.h"
#include "../Device.h"
#include "../video/RenderTechnique.h"
#include "../video/Material.h"
#include "../video/Texture.h"
#include "../video/TextureLoader/TextureLoader.h"
#include "../scene/ModelBase.h"
#include "../scene/ModelLoader/ModelLoader.h"

namespace sh
{
	ResourceManager::ResourceManager()
	{
		video::TextureLoader::CreateInstance();
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
			if (m_textures[i]->GetFileName() == fileName)
				return m_textures[i];
		}

		video::TexturePtr texture = video::TextureLoader::GetInstance()->Load(fileName);
		texture->SetFileName(fileName);
		m_textures.push_back(texture);
		return texture;

#if 0
		// Find file in file system
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		io::FileInfo textureFileInfo = fs->FindFile(fileName);

		// If file exists in file system then load
		if (textureFileInfo.name != "")
		{
			video::TexturePtr texture = m_textureLoader->Load(textureFileInfo.absolutePath);
			texture->SetFileName(textureFileInfo.name);
			m_textures.push_back(texture);

			return texture;
		}
#endif

		return nullptr;
	}

	video::TexturePtr ResourceManager::GetCubeTexture(const std::vector<String>& faces)
	{
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		
		std::vector<String> facesPathes;
		for (auto face : faces)
		{
			io::FileInfo textureFileInfo = fs->FindFile(face);
			if (textureFileInfo.name != "")
			{
				facesPathes.push_back(textureFileInfo.absolutePath);
			}
		}
		video::TexturePtr texture = video::TextureLoader::GetInstance()->LoadCube(facesPathes);
		
		return texture;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	video::RenderTechniquePtr ResourceManager::GetRenderTechnique(const String& fileName)
	{
		// Check if RenderTechnique already exists in techniques pool
		for (size_t i = 0, sz = m_renderTechniques.size(); i < sz; ++i)
		{
			if (m_renderTechniques[i]->GetFileName() == fileName)
				return m_renderTechniques[i];
		}
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		io::File file = fs->LoadFile(fileName);

		video::RenderTechniquePtr rt(new video::RenderTechnique());
		rt->Load(file.GetData());
		rt->SetFileName(fileName);
		m_renderTechniques.push_back(rt);
		return rt;

#if 0

#if defined SHADOW_WINDOWS
		// Find file in file system
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem(); 
		io::FileInfo rtFileInfo = fs->FindFile(fileName);

		// If file exists in file system then load
		if (rtFileInfo.name != "")
		{
			video::RenderTechniquePtr rt(new video::RenderTechnique());
			rt->Load(rtFileInfo.absolutePath);
			rt->SetFileName(rtFileInfo.name);
			m_renderTechniques.push_back(rt);
			return rt;
		}
#elif defined SHADOW_ANDROID
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		std::vector<char> buffer;
		buffer = fs->ReadFile(fileName);

		video::RenderTechniquePtr rt(new video::RenderTechnique());
		rt->Load(buffer);
		rt->SetFileName(fileName);
		m_renderTechniques.push_back(rt);
		return rt;
#endif

#endif

		return nullptr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ResourceManager::AddMaterial(const video::MaterialPtr& material)
	{
		for (size_t i = 0U, sz = m_materials.size(); i < sz; ++i)
		{
			if (m_materials[i]->GetName() == material->GetName())
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
			if (m_materials[i]->GetName() == materialName)
			{
				return m_materials[i];
			}
		}
		return nullptr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	scene::ModelBasePtr ResourceManager::GetModelBase(const String& fileName)
	{
		// Check if Modell already exists in models pool
		for (size_t i = 0, sz = m_models.size(); i < sz; ++i)
		{
			if (m_models[i]->GetFileName() == fileName)
				return m_models[i];
		}

		scene::ModelBasePtr model = scene::ModelLoader::GetInstance()->Load(fileName);
		model->SetFileName(fileName);
		m_models.push_back(model);
		return model;

#if 0
		// Find file in file system
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		io::FileInfo modelFileInfo = fs->FindFile(fileName);

		// If file exists in file system then load
		if (modelFileInfo.name != "")
		{
			scene::ModelBasePtr model = scene::ModelLoader::GetInstance()->Load(modelFileInfo.absolutePath);
			model->SetFileName(modelFileInfo.name);
			m_models.push_back(model);
			return model;
		}
#endif
		return nullptr;
	}

}