#ifndef SHADOW_RESOURCE_MANAGER_INCLUDE
#define SHADOW_RESOURCE_MANAGER_INCLUDE

#include "Resource.h"



namespace sh
{
	namespace video
	{
		class Texture;
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

		video::Texture* GetTexture(const String& fileName);
		video::Texture* GetCubeTexture(const std::vector<String>& faces);
		video::RenderTechnique* GetRenderTechnique(const String& fileName);
		scene::ModelBase* GetModelBase(const String& fileName);

	private:
		std::vector<video::RenderTechnique*> m_renderTechniques;
		std::vector<video::Texture*> m_textures;
		std::vector<scene::ModelBase*> m_models;
	};
}

#endif