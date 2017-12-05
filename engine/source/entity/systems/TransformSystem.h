#ifndef SHADOW_TRANSFORM_SYSTEM_INCLUDE
#define SHADOW_TRANSFORM_SYSTEM_INCLUDE

#include "../System.h"

namespace sh
{
	class SHADOW_API TransformSystem : public System
	{
	public:
		virtual void RegisterEntity(Entity* entity) override;
		virtual void RegisterComponent(Component* component) override;
		virtual void AddEntity(Entity* entity) override;
		virtual void RemoveEntity(Entity* entity) override;
		virtual void Update(f32 deltaTime) override;
		virtual void Clear() override;

	private:
		std::vector<Entity*> m_entities;
	};
}

#endif