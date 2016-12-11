#include "ResourceManager.h"
#include "../Device.h"
#include "../video/RenderTechnique.h"
#include "../video/Texture.h"
#include "../video/TextureLoader/TextureLoader.h"
#include "../scene/ModelBase.h"
#include "../scene/ModelLoader/ModelLoader.h"

namespace sh
{
	ResourceManager::ResourceManager()
	{
		m_textureLoader = new video::TextureLoader();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	ResourceManager::~ResourceManager()
	{
		delete m_textureLoader;
		m_textureLoader = nullptr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ResourceManager::Init()
	{
		
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	video::Texture* ResourceManager::GetTexture(const String& fileName)
	{
		// Check if Texture already exists in textures pool
		for (size_t i = 0, sz = m_textures.size(); i < sz; ++i)
		{
			if (m_textures[i]->GetFileName() == fileName)
				return m_textures[i];
		}

		// Find file in file system
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		io::FileInfo textureFileInfo = fs->FindFile(fileName);

		// If file exists in file system then load
		if (textureFileInfo.name != "")
		{
			video::Texture* texture = m_textureLoader->Load(textureFileInfo.absolutePath);
			texture->SetFileName(textureFileInfo.name);
			m_textures.push_back(texture);

			return texture;
		}

		return nullptr;
	}

	video::Texture* ResourceManager::GetCubeTexture(const std::vector<String>& faces)
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
		video::Texture* texture = m_textureLoader->LoadCube(facesPathes);
		
		return texture;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	video::RenderTechnique* ResourceManager::GetRenderTechnique(const String& fileName)
	{
		// Check if RenderTechnique already exists in techniques pool
		for (size_t i = 0, sz = m_renderTechniques.size(); i < sz; ++i)
		{
			if (m_renderTechniques[i]->GetFileName() == fileName)
				return m_renderTechniques[i];
		}

		// Find file in file system
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		io::FileInfo rtFileInfo = fs->FindFile(fileName);

		// If file exists in file system then load
		if (rtFileInfo.name != "")
		{
			video::RenderTechnique* rt = new video::RenderTechnique();
			rt->Load(rtFileInfo.absolutePath);
			rt->SetFileName(rtFileInfo.name);
			m_renderTechniques.push_back(rt);
			return rt;
		}

		return nullptr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	scene::ModelBase* ResourceManager::GetModelBase(const String& fileName)
	{
		// Check if Modell already exists in models pool
		for (size_t i = 0, sz = m_models.size(); i < sz; ++i)
		{
			if (m_models[i]->GetFileName() == fileName)
				return m_models[i];
		}

		// Find file in file system
		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		io::FileInfo modelFileInfo = fs->FindFile(fileName);

		// If file exists in file system then load
		if (modelFileInfo.name != "")
		{
			scene::ModelBase* model = scene::ModelLoader::GetInstance()->Load(modelFileInfo.absolutePath);
			model->SetFileName(modelFileInfo.name);
			m_models.push_back(model);
			return model;
		}

		return nullptr;
	}

}