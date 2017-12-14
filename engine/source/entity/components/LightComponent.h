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

	class SHADOW_API LightComponent : public Component
	{
		COMPONENT(LightComponent)
	public:
		LightComponent();
		virtual ~LightComponent();

		static void RegisterObject();

		virtual Component* Clone() override;

		bool NeedsToRecalculate() const { return m_needsToRecalculate; }		

		void SetPosition(const math::Vector3& position);
		const math::Vector3& GetPosition() const;

		void SetDirection(const math::Vector3& direction);
		const math::Vector3& GetDirection() const;

		void SetColor(const video::Color& color);
		const video::Color& GetColor() const;

		scene::Light::Type GetType() const;
		void SetType(scene::Light::Type type);

	private:
		scene::Light* m_light = nullptr;
		bool m_needsToRecalculate = true;

		friend class LightSystem;
	};
}

#endif