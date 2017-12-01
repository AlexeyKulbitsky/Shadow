#ifndef SHADOW_LIGHT_SYSTEM_INCLUDE
#define SHADOW_LIGHT_SYSTEM_INCLUDE

#include "../System.h"

namespace sh
{
	class SHADOW_API LightSystem : public System
	{
	public:
		LightSystem();
		virtual ~LightSystem();

		virtual void RegisterEntity(Entity* entity) override;
		virtual void AddEntity(Entity* entity) override;
		virtual void RemoveEntity(Entity* entity) override;
		virtual void Update(f32 deltaTime) override;
		virtual void Clear() override;

	private:
		std::vector<Entity*> m_entities;
		video::RenderBatchManager* m_batchManager;
	};
}

#endif