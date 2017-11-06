#include "LightSystem.h"
#include "../Entity.h"
#include "../components/LightComponent.h"
#include "../components/TransformComponent.h"
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
		if (entity->GetComponent<LightComponent>() &&
			entity->GetComponent<TransformComponent>())
		{
			AddEntity(entity);
		}
	}

	//////////////////////////////////////////////////////////////////

	void LightSystem::AddEntity(Entity* entity)
	{
		auto it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it != m_entities.end())
			return;
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
			if (true)//lightComponent->m_needsToRecalculate)
			{
				auto transformComponent = entity->GetComponent<TransformComponent>();
				if (transformComponent)
				{
					switch (lightComponent->GetType())
					{
						case scene::Light::Type::DIRECTIONAL:
						{
							auto direction = transformComponent->GetRotation() * math::Vector3f(0.0, 1.0, 0.0f);
							direction.Normalize();
							lightComponent->SetDirection(direction);
						}
							break;
						case scene::Light::Type::AMBIENT:
							break;
						case scene::Light::Type::POINT:
							break;
						case scene::Light::Type::SPOT:
							break;
						default:
							break;
					}
				}

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