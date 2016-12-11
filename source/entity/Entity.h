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
		Component* GetComponent(Component::Type type);

	private:
		std::vector<Component*> m_components;
	};
}

#endif