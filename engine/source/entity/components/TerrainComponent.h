#ifndef SHADOW_TERRAIN_COMPONENT_INCLUDE
#define SHADOW_TERRAIN_COMPONENT_INCLUDE

#include "../Component.h"

namespace sh
{
	namespace video
	{
		class Image;
	}

	class TerrainComponent : public Component
	{
		SHADOW_OBJECT(TerrainComponent)
		COMPONENT
	public:
		virtual ~TerrainComponent();

		static void RegisterObject();

		const scene::ModelPtr& GetModel() { return m_model; }

		void SetHeightmapProperty(const ResourceRef& value);
		ResourceRef GetHeightmapProperty() const;

	private:
		void SetHightMapFromImage(const video::Image& image);
		void CalculateNormals(std::vector<float>& vertexArray, 
							  const std::vector<u32>& indexArray,
							  const video::VertexDeclarationPtr& vertexDeclaration);

	private:
		String m_heightmapName;
		scene::ModelPtr m_model;
	};

} // sh

#endif