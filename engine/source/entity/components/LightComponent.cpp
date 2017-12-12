#include "LightComponent.h"
#include "../../scene/Light.h"
#include "../../video/Driver.h"
#include "../../Device.h"
#include "../../serialization/ObjectFactory.h"

namespace sh
{
	static const char* typeNames[] =
	{
		"Point",
		"Directional",
		"Spot",
		"Ambient"
	};

	static const std::vector<String> lightTypeNames = 
	{
		"Point",
		"Directional",
		"Spot",
		"Ambient"
	};

	LightComponent::LightComponent()
	{
		m_light = new scene::Light();
	}

	//////////////////////////////////////////////////////////

	LightComponent::~LightComponent()
	{
		if (m_light)
		{
			delete m_light;
			m_light = nullptr;
		}
	}

	//////////////////////////////////////////////////////////

	void LightComponent::RegisterObject()
	{
		ObjectFactory::GetInstance()->RegisterFactory<LightComponent>("Components");
		S_ACCESSOR_PROPERTY("Color", GetColor, SetColor);
		S_ENUM_ACCESSOR_PROPERTY("Type", GetType, SetType, lightTypeNames);
	}

	//////////////////////////////////////////////////////////

	void LightComponent::SetPosition(const math::Vector3& position)
	{
		m_light->SetPosition(position);
		m_needsToRecalculate = true;
	}

	//////////////////////////////////////////////////////////

	const math::Vector3& LightComponent::GetPosition() const
	{
		return m_light->GetPosition();
	}

	//////////////////////////////////////////////////////////

	void LightComponent::SetDirection(const math::Vector3& direction)
	{
		m_light->SetDirection(direction);
		m_needsToRecalculate = true;
	}

	//////////////////////////////////////////////////////////

	const math::Vector3& LightComponent::GetDirection() const
	{
		return m_light->GetDirection();
	}

	//////////////////////////////////////////////////////////

	void LightComponent::SetColor(const video::Color& color)
	{
		m_light->SetColor(color);
		m_needsToRecalculate = true;
	}

	//////////////////////////////////////////////////////////

	const video::Color& LightComponent::GetColor() const
	{
		return m_light->GetColor();
	}

	//////////////////////////////////////////////////////////

	scene::Light::Type LightComponent::GetType() const
	{
		return m_light->GetType();
	}

	//////////////////////////////////////////////////////////

	void LightComponent::SetType(scene::Light::Type type)
	{
		m_light->SetType(type);
		m_needsToRecalculate = true;
	}

	//////////////////////////////////////////////////////////
}