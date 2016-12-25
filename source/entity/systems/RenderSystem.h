#ifndef SHADOW_RENDER_SYSTEM_INCLUDE
#define SHADOW_RENDER_SYSTEM_INCLUDE

#include "../System.h"

namespace sh
{
	namespace video
	{
		class CommandPool;
	}

	class RenderSystem : public System
	{
	public:
		RenderSystem();
		virtual ~RenderSystem();

		virtual void AddEntity(Entity* entity) override;
		virtual void RemoveEntity(Entity* entity) override;
		virtual void Update(f32 deltaTime) override;

	private:
		std::vector<Entity*> m_entities;
		video::CommandPool* m_commandPool;
	};
}

#endif