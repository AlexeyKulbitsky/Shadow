#ifndef SHADOW_SYSTEM_INCLUDE
#define SHADOW_SYSTEM_INCLUDE

#include "../Globals.h"
#include "Component.h"

namespace sh
{
	class Entity;

	class System
	{
	public:
		virtual void RegisterEntity(Entity* entity);
		virtual void AddEntity(Entity* entity);
		virtual void RemoveEntity(Entity* entity);
		virtual void Update(f32 deltaTime);
		//virtual void AddComponentType(Component::Type type);
		virtual void Clear();
	
	protected:
		//std::vector<Component::Type> m_componentTypes;
	};
}

#endif