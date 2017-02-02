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
		virtual Component::Type GetType() const override { return Component::Type::LIGHT; }
		virtual void Load(const pugi::xml_node &node) override;
		bool NeedsToRecalculate() const { return m_needsToRecalculate; }		

		void SetPosition(const math::Vector3f& position);
		const math::Vector3f& GetPosition() const;

		void SetDirection(const math::Vector3f& direction);
		const math::Vector3f& GetDirection() const;

		void SetColor(math::Vector3f& color);
		const math::Vector3f&  GetColor() const;

	private:
		scene::Light* m_light;
		bool m_needsToRecalculate;

		friend class LightSystem;
	};
}

#endif