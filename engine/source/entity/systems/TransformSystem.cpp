#include "TransformSystem.h"
#include "../Entity.h"
#include "../components/TransformComponent.h"
#include "../components/RenderComponent.h"
#include "../../scene/Model.h"

namespace sh
{
	void TransformSystem::RegisterEntity(Entity* entity)
	{
		if (entity->GetComponent<TransformComponent>())
		{
			AddEntity(entity);
		}
	}

	//////////////////////////////////////////////////////////////////

	void TransformSystem::AddEntity(Entity* entity)
	{
		auto it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it != m_entities.end())
			return;
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
			TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
			if (transformComponent->NeedsToRecalculateWorldMatrix())
			{
				transformComponent->m_worldMatrix.SetIdentity();
				const math::Vector3& translation = transformComponent->m_position;
				const math::Vector3& scale = transformComponent->m_scale;
				const math::Quaternion& rotation = transformComponent->m_rotation;

				math::Matrix4 scaleMatrix;
				scaleMatrix.SetIdentity();
				scaleMatrix.SetScale(scale);
				transformComponent->m_worldMatrix.SetTranslation(translation);
				transformComponent->m_worldMatrix = transformComponent->m_worldMatrix * rotation.GetAsMatrix4() * scaleMatrix;
				
				RenderComponent* renderComponent = entity->GetComponent<RenderComponent>();
				if (renderComponent)
				{
					auto box = renderComponent->GetModel()->GetInitialBoundingBox().GetTransformed(transformComponent->m_worldMatrix);
					renderComponent->GetModel()->SetBoundingBox(box);
				}

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