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
			TransformComponent* transformComponent = static_cast<TransformComponent*>( entity->GetComponent(Component::Type::Transform) );
			if (transformComponent->NeedsToRecalculateWorldMatrix())
			{
				transformComponent->m_worldMatrix.SetIdentity();
				const math::Vector3f& translation = transformComponent->m_position;
				const math::Vector3f& scale = transformComponent->m_scale;
				const math::Quaternionf& rotation = transformComponent->m_rotation;

				math::Matrix4f scaleMatrix;
				scaleMatrix.SetIdentity();
				scaleMatrix.SetScale(scale);
				transformComponent->m_worldMatrix.SetTranslation(translation);
				transformComponent->m_worldMatrix = transformComponent->m_worldMatrix * rotation.GetAsMatrix4() * scaleMatrix;
				
				transformComponent->m_needsToRecalculateWorldMatrix = false;
			}
		}
	}

	//////////////////////////////////////////////////////////////////

	void TransformSystem::Clear()
	{
		m_entities.clear();
	}

	//////////////////////////////////////////////////////////////////

}