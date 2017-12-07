#ifndef SHADOW_SYSTEM_INCLUDE
#define SHADOW_SYSTEM_INCLUDE

#include "../Globals.h"
#include "Component.h"

namespace sh
{
	class Entity;
	class Component;

	class SHADOW_API System
	{
	public:
		virtual void RegisterEntity(Entity* entity);
		virtual void RegisterComponent(Component* component);
		virtual void AddEntity(Entity* entity);
		virtual void RemoveEntity(Entity* entity);
		virtual void Update(f32 deltaTime);
		//virtual void AddComponentType(Component::Type type);
		virtual void Clear();

		void Activate(bool activate) { m_activated = activate; }
		bool IsActivated() const { return m_activated; }
	
	protected:
		bool m_activated = false;
		//std::vector<Component::Type> m_componentTypes;
	};
}

#endif