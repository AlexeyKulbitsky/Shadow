#ifndef SHADOW_LIGHT_COMPONENT_INCLUDE
#define SHADOW_LIGHT_COMPONENT_INCLUDE

#include "../Component.h"
#include "../../scene/Light.h"

namespace sh
{
	namespace scene
	{
		class Light;
	}

	class LightComponent : public Component
	{
		COMPONENT
	public:
		virtual ~LightComponent();
		virtual void Load(const pugi::xml_node &node) override;
		virtual void Save(pugi::xml_node &parent) override;

		bool NeedsToRecalculate() const { return m_needsToRecalculate; }		

		void SetPosition(const math::Vector3f& position);
		const math::Vector3f& GetPosition() const;

		void SetDirection(const math::Vector3f& direction);
		const math::Vector3f& GetDirection() const;

		void SetColor(math::Vector3f& color);
		const math::Vector3f&  GetColor() const;

		scene::Light::Type GetType() const;
		void SetType(scene::Light::Type type);

	private:
		scene::Light* m_light = nullptr;
		bool m_needsToRecalculate = true;

		friend class LightSystem;
	};
}

#endif