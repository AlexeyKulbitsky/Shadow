#ifndef SHADOW_SYSTEM_INCLUDE
#define SHADOW_SYSTEM_INCLUDE

#include "../Globals.h"
#include "Component.h"

namespace sh
{
	class Entity;
	class Component;

	enum class SystemState
	{
		Running,
		Paused,
		Stopped
	};

	class SHADOW_API System
	{
	public:
		virtual void RegisterEntity(Entity* entity);
        virtual void UnregisterEntity(Entity* entity);
		virtual void RegisterComponent(Component* component);
		virtual void AddEntity(Entity* entity);
		virtual void RemoveEntity(Entity* entity);
		virtual void Update(f32 deltaTime);
		virtual void Clear();

		void Activate(bool activate) { m_activated = activate; }
		bool IsActivated() const { return m_activated; }

		void SetState(SystemState state) { m_state = state; }
		SystemState GetState() const { return m_state; }
	
	protected:
		bool m_activated = false;
		SystemState m_state = SystemState::Running;
	};
}

#endif
