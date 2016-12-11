#ifndef SHADOW_SYSTEM_INCLUDE
#define SHADOW_SYSTEM_INCLUDE

#include "../Globals.h"

namespace sh
{
	class Entity;

	class System
	{
	public:
		virtual void AddEntity(Entity* entity);
		virtual void RemoveEntity(Entity* entity);
		virtual void Update(f32 deltaTime);
	};
}

#endif