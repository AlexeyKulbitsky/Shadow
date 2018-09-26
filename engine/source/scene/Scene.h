#ifndef SHADOW_SCENE_INCLUDE
#define SHADOW_SCENE_INCLUDE

#include "Node.h"
#include "../entity/System.h"

namespace sh
{

	class Entity;
	class System;
		
namespace scene
{
	
	class SHADOW_API Scene : public Node
	{
	public:
		Scene();
		virtual ~Scene();

		void AddEntity(Entity* entity);
		size_t GetEntitiesCount() const;
		Entity* GetEntity(size_t index);
		void RegisterEntity(Entity* entity);
        void UnregisterEntity(Entity* entity);
		void SetSystemsState(SystemState state);

		void Update(float deltaTime = 0.0f);
		void Clear();

		Scene* Clone();

	private:
		std::vector<System*> m_systems;
		std::vector<Entity*> m_entities;
		std::vector<System*> m_switchableSystems;
	};

} // scene

} // sh

#endif
