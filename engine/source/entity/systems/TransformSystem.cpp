#include "TransformSystem.h"
#include "../Entity.h"
#include "../components/TransformComponent.h"

namespace sh
{
	void TransformSystem::AddEntity(Entity* entity)
	{
		m_entities.push_back(entity);
	}

	//////////////////////////////////////////////////////////////////

	void TransformSystem::RemoveEntity(Entity* entity)
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

	void TransformSystem::Update(f32 deltaTime)
	{
		for (auto entity : m_entities)
		{
			TransformComponent* transformComponent = static_cast<TransformComponent*>( entity->GetComponent(Component::Type::TRANSFORM) );
			if (transformComponent->NeedsToRecalculateWorldMatrix())
			{
				transformComponent->m_worldMatrix.SetIdentity();
				math::Vector3f translation = transformComponent->m_position;
				math::Vector3f scale = transformComponent->m_scale;

				transformComponent->m_worldMatrix.SetScale(scale);
				transformComponent->m_worldMatrix.SetTranslation(translation);
				transformComponent->m_needsToRecalculateWorldMatrix = false;
			}
		}
	}

	//////////////////////////////////////////////////////////////////

}