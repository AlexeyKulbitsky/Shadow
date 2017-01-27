#ifndef SHADOW_RESOURCE_MANAGER_INCLUDE
#define SHADOW_RESOURCE_MANAGER_INCLUDE

#include "Resource.h"



namespace sh
{
	namespace video
	{
		class Texture;
		class TextureLoader;
		class RenderTechnique;
	}

	namespace scene
	{
		class ModelBase;
	}

	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();
		void Init();
		void Unload();

		video::Texture* GetTexture(const String& fileName);
		video::Texture* GetCubeTexture(const std::vector<String>& faces);
		video::RenderTechnique* GetRenderTechnique(const String& fileName);
		scene::ModelBase* GetModelBase(const String& fileName);

		video::TextureLoader* GetTextureLoader() const { return m_textureLoader; }

	private:
		std::vector<video::RenderTechnique*> m_renderTechniques;
		std::vector<video::Texture*> m_textures;
		std::vector<scene::ModelBase*> m_models;

		// Managers for loading assets
		video::TextureLoader* m_textureLoader;
	};
}

#endif