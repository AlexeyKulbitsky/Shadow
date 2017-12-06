#ifndef SHADOW_SCRIPT_SYSTEM_INCLUDE
#define SHADOW_SCRIPT_SYSTEM_INCLUDE

#include "../System.h"

namespace sh
{

	class SHADOW_API ScriptSystem : public System
	{
	public:
		ScriptSystem();
		virtual ~ScriptSystem();

		virtual void RegisterEntity(Entity* entity) override;
		virtual void RegisterComponent(Component* component) override;
		virtual void AddEntity(Entity* entity) override;
		virtual void RemoveEntity(Entity* entity) override;
		virtual void Update(f32 deltaTime) override;
		virtual void Clear() override;

	private:
		std::vector<Entity*> m_entities;
	};

} // sh

#endif
