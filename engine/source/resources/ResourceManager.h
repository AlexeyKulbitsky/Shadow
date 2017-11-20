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

	namespace io
	{
		struct FolderInfo;
	}

	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		ResourceManager();
		~ResourceManager();
		void Init();
		void Unload();

		video::TexturePtr GetTexture(const String& fileName);
		video::TexturePtr GetCubeTexture(const std::vector<String>& faces);
		video::RenderTechniquePtr GetRenderTechnique(const String& fileName);
		void AddMaterial(const video::MaterialPtr& material);
		size_t GetMaterialsCount() const { return m_materials.size(); }
		video::MaterialPtr GetMaterial(const String& materialName);
		video::MaterialPtr GetMaterial(size_t i) { return m_materials[i]; }
		scene::ModelBasePtr GetModelBase(const String& fileName);

		const video::MaterialPtr& GetDefaultMaterial();
		const video::TexturePtr& GetDefaultTexture();

		void UpdateResourceGroups();

	private:
		void UpdateRecursive(const SPtr<io::FolderInfo>& fsComponent,
			const std::vector<String>& imageExtensions,
			const std::vector<String>& modelExtensions);

	private:
		std::vector<video::RenderTechniquePtr> m_renderTechniques;
		std::vector<video::TexturePtr> m_textures;
		std::vector<scene::ModelBasePtr> m_models;
		std::vector<video::MaterialPtr> m_materials;
		video::MaterialPtr m_defaultMaterial;

		// All available resources sorted by groups (Models, Materials, Textures, etc.)
		Map<String, ResourceRefList> m_resourceGroups;
	};
}

#endif