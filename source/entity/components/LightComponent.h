#ifndef SHADOW_LIGHT_COMPONENT_INCLUDE
#define SHADOW_LIGHT_COMPONENT_INCLUDE

#include "../Component.h"

namespace sh
{
	namespace scene
	{
		class Light;
	}

	class LightComponent : public Component
	{
	public:
		virtual Type GetType() const { return Component::Type::LIGHT; }
		virtual void Load(const pugi::xml_node &node) override;

	private:
		scene::Light* m_light;
	};
}

#endif