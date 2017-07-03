#ifndef SHADOW_TERRAIN_COMPONENT_INCLUDE
#define SHADOW_TERRAIN_COMPONENT_INCLUDE

#include "../Component.h"

namespace sh
{

	class TerrainComponent : public Component
	{
	public:
		virtual ~TerrainComponent();
		virtual Type GetType() const override { return Component::Type::Terrain; }
		virtual void Load(const pugi::xml_node &node) override;
		virtual void Save(pugi::xml_node &parent) override;

		const scene::ModelPtr& GetModel() { return m_model; }

	private:
		scene::ModelPtr m_model;
	};

} // sh

#endif