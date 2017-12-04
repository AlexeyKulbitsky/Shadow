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

}