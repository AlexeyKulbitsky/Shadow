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
		SHADOW_OBJECT(LightComponent)
		COMPONENT
	public:
		LightComponent();
		virtual ~LightComponent();

		static void RegisterObject();

		bool NeedsToRecalculate() const { return m_needsToRecalculate; }		

		void SetPosition(const math::Vector3f& position);
		const math::Vector3f& GetPosition() const;

		void SetDirection(const math::Vector3f& direction);
		const math::Vector3f& GetDirection() const;

		void SetColor(const math::Vector3f& color);
		const math::Vector3f& GetColor() const;

		scene::Light::Type GetType() const;
		void SetType(scene::Light::Type type);

	private:
		scene::Light* m_light = nullptr;
		bool m_needsToRecalculate = true;

		friend class LightSystem;
	};
}

#endif