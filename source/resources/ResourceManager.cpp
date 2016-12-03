#include "ResourceManager.h"
#include "../Device.h"
#include "../video/RenderTechnique.h"
#include "../video/Texture.h"

namespace sh
{
	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{

	}

	void ResourceManager::Init()
	{
		
	}

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
			video::Texture* texture = Device::GetInstance()->GetDriver()->CreateTexture();
			texture->Load(textureFileInfo.absolutePath);
			texture->SetFileName(textureFileInfo.name);
			m_textures.push_back(texture);
			return texture;
		}

		return nullptr;
	}

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

}