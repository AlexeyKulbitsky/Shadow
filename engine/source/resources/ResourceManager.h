#ifndef SHADOW_RESOURCE_MANAGER_INCLUDE
#define SHADOW_RESOURCE_MANAGER_INCLUDE

#include "Resource.h"



namespace sh
{
	namespace video
	{
		class TextureLoader;
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

		video::TexturePtr GetTexture(const String& fileName);
		video::TexturePtr GetCubeTexture(const std::vector<String>& faces);
		video::RenderTechniquePtr GetRenderTechnique(const String& fileName);
		scene::ModelBasePtr GetModelBase(const String& fileName);

		video::TextureLoader* GetTextureLoader() const { return m_textureLoader; }

	private:
		std::vector<video::RenderTechniquePtr> m_renderTechniques;
		std::vector<video::TexturePtr> m_textures;
		std::vector<scene::ModelBasePtr> m_models;

		// Managers for loading assets
		video::TextureLoader* m_textureLoader;
	};
}

#endif