#ifndef SHADOW_ENTITY_INCLUDE
#define SHADOW_ENTITY_INCLUDE

#include "../Globals.h"
#include "Component.h"

namespace sh
{
	class Entity
	{
	public:
		Entity();
		~Entity();

		void AddComponent(Component* component);
		void SetComponent(Component::Type type, Component* component);
		Component* GetComponent(Component::Type type);

		bool IntersectsRay(const math::Vector3f& origin, const math::Vector3f& direction);

	private:	
		Component* m_components[static_cast<size_t>(Component::Type::COUNT)];
	};
}

#endif