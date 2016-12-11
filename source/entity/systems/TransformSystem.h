#ifndef SHADOW_TRANSFORM_SYSTEM_INCLUDE
#define SHADOW_TRANSFORM_SYSTEM_INCLUDE

#include "../System.h"

namespace sh
{
	class TransformSystem : public System
	{
	public:
		virtual void AddEntity(Entity* entity) override;
		virtual void RemoveEntity(Entity* entity) override;
		virtual void Update(f32 deltaTime) override;

	private:
		std::vector<Entity*> m_entities;
	};
}

#endif