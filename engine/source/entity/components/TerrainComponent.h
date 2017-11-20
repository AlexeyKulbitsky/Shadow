#ifndef SHADOW_TERRAIN_COMPONENT_INCLUDE
#define SHADOW_TERRAIN_COMPONENT_INCLUDE

#include "../Component.h"

namespace sh
{

	class TerrainComponent : public Component
	{
		SHADOW_OBJECT(TerrainComponent)
		COMPONENT
	public:
		virtual ~TerrainComponent();
		//virtual Type GetType() const override { return Component::Type::Terrain; }
		virtual void Load(const pugi::xml_node &node) override;
		virtual void Save(pugi::xml_node &parent) override;

		const scene::ModelPtr& GetModel() { return m_model; }

	private:
		void CalculateNormals(std::vector<float>& vertexArray, 
							  const std::vector<u32>& indexArray,
							  const video::VertexDeclarationPtr& vertexDeclaration);

	private:
		String m_heightmapName;
		scene::ModelPtr m_model;
	};

} // sh

#endif