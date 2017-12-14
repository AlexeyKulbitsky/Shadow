#include "TransformComponent.h"
#include "../../serialization/ObjectFactory.h"

namespace sh
{
	TransformComponent::TransformComponent()
	{
		m_position = math::Vector3(0.0f);
		m_rotation.SetIndentity();
		m_scale = math::Vector3(1.0f);
		m_needsToRecalculateWorldMatrix = false;
	}

	/////////////////////////////////////////////////////////////////

	TransformComponent::~TransformComponent()
	{

	}

	/////////////////////////////////////////////////////////////////

	void TransformComponent::RegisterObject()
	{
		ObjectFactory::GetInstance()->RegisterFactory<TransformComponent>("Components");
		S_ACCESSOR_PROPERTY("Position", GetPosition, SetPosition);
		S_ACCESSOR_PROPERTY("Rotation", GetRotation, SetRotation);
		S_ACCESSOR_PROPERTY("Scale", GetScale, SetScale);
	}

	/////////////////////////////////////////////////////////////////

	sh::Component* TransformComponent::Clone()
	{
		TransformComponent* component = new TransformComponent();

		component->m_worldMatrix = m_worldMatrix;
		component->m_position = m_position;
		component->m_rotation = m_rotation;
		component->m_scale = m_scale;
		component->m_translateMatrix = m_translateMatrix;
		component->m_rotationMatrix = m_rotationMatrix;
		component->m_scaleMatrix = m_scaleMatrix;
		component->m_needsToRecalculateWorldMatrix = m_needsToRecalculateWorldMatrix;

		return component;
	}

	/////////////////////////////////////////////////////////////////

}