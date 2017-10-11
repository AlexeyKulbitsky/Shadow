#include "LightSystem.h"
#include "../Entity.h"
#include "../components/LightComponent.h"
#include "../../scene/Light.h"

#include "../../video/RenderBatchManager.h"

namespace sh
{
	LightSystem::LightSystem()
	{
		m_batchManager = video::RenderBatchManager::GetInstance();
	}

	//////////////////////////////////////////////////////////////////

	LightSystem::~LightSystem()
	{

	}

	//////////////////////////////////////////////////////////////////

	void LightSystem::RegisterEntity(Entity* entity)
	{
		if (entity->GetComponent<LightComponent>())
		{
			AddEntity(entity);
		}
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
			auto lightComponent = entity->GetComponent<LightComponent>();	
			if (lightComponent->m_needsToRecalculate)
			{
				lightComponent->m_needsToRecalculate = false;

				m_batchManager->UpdateLight(lightComponent->m_light);
			}
		}
	}

	//////////////////////////////////////////////////////////////////

	void LightSystem::Clear()
	{
		m_entities.clear();
	}

	//////////////////////////////////////////////////////////////////
}