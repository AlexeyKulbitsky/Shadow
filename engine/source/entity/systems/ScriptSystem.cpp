#include "ScriptSystem.h"

#include "../Entity.h"
#include "../components/ScriptComponent.h"

namespace sh
{

	ScriptSystem::ScriptSystem()
	{

	}

	ScriptSystem::~ScriptSystem()
	{

	}

	void ScriptSystem::RegisterEntity(Entity* entity)
	{
		if (entity->GetComponent<ScriptComponent>())
		{
			AddEntity(entity);
		}
	}

	void ScriptSystem::RegisterComponent(Component* component)
	{

	}

	void ScriptSystem::AddEntity(Entity* entity)
	{
		auto it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it != m_entities.end())
			return;
		m_entities.push_back(entity);
	}

	void ScriptSystem::RemoveEntity(Entity* entity)
	{

	}

	void ScriptSystem::Update(f32 deltaTime)
	{
		for (auto entity : m_entities)
		{
			auto scriptComponent = entity->GetComponent<ScriptComponent>();
			if (!scriptComponent)
				continue;

			scriptComponent->Update(deltaTime);
		}
	}

	void ScriptSystem::Clear()
	{

	}

} // sh