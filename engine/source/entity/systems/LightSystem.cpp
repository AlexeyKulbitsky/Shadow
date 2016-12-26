#include "LightSystem.h"
#include "../Entity.h"
#include "../components/LightComponent.h"

namespace sh
{
	LightSystem::LightSystem()
	{

	}

	//////////////////////////////////////////////////////////////////

	LightSystem::~LightSystem()
	{

	}

	//////////////////////////////////////////////////////////////////

	void LightSystem::AddEntity(Entity* entity)
	{
		m_entities.push_back(entity);
	}

	//////////////////////////////////////////////////////////////////

	void LightSystem::RemoveEntity(Entity* entity)
	{
		for (size_t i = 0, sz = m_entities.size(); i < sz; ++i)
		{
			if (m_entities[i] == entity)
			{
				m_entities.erase(m_entities.begin() + i);
				return;
			}
		}
	}

	//////////////////////////////////////////////////////////////////

	void LightSystem::Update(f32 deltaTime)
	{
		for (auto entity : m_entities)
		{
			LightComponent* lightComponent = static_cast<LightComponent*>(entity->GetComponent(Component::Type::LIGHT));	
			if (lightComponent->m_needsToRecalculate)
			{
				lightComponent->m_needsToRecalculate = false;
			}
		}
	}

	//////////////////////////////////////////////////////////////////
}