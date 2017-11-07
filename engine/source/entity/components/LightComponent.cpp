#include "LightComponent.h"
#include "../../scene/Light.h"
#include "../../video/Driver.h"
#include "../../Device.h"

namespace sh
{
	LightComponent::~LightComponent()
	{
		if (m_light)
		{
			delete m_light;
			m_light = nullptr;
		}
	}

	//////////////////////////////////////////////////////////

	void LightComponent::Load(const pugi::xml_node &node)
	{
		// Type
		pugi::xml_node lightTypeNode = node.child("type");
		if (lightTypeNode)
		{
			pugi::xml_attribute valAttr = lightTypeNode.attribute("val");
			String valString = valAttr.as_string();

			scene::Light::Type lightType = scene::Light::Type::DIRECTIONAL;

			m_light = new scene::Light();
			if (valString == "directional")
			{
				lightType = scene::Light::Type::DIRECTIONAL;
				m_light->SetType(lightType);
			}


			// Color
			pugi::xml_node colorNode = node.child("color");
			if (colorNode)
			{
				pugi::xml_attribute valAttr = colorNode.attribute("val");
				String colorString = valAttr.as_string();
				std::istringstream in(colorString);
				float val = 0.0f;
				std::vector<float> temp;
				while (in >> val) temp.push_back(val);
				math::Vector3f color(0.0f);
				color.x = temp[0];
				color.y = temp[1];
				color.z = temp[2];

				m_light->SetColor(color);
			}

		}
	}

	//////////////////////////////////////////////////////////

	void LightComponent::Save(pugi::xml_node &parent)
	{
		pugi::xml_node componentNode = parent.append_child("component");
		componentNode.append_attribute("name").set_value("light");

		pugi::xml_node typeNode = componentNode.append_child("type");
		sh::scene::Light::Type type = m_light->GetType();
		switch (type)
		{
			case sh::scene::Light::Type::DIRECTIONAL:
			{
				typeNode.append_attribute("val").set_value("directional");
			}
			break;
			default:
			break;
		}

		const sh::math::Vector3f& color = m_light->GetColor();
		std::ostringstream out;
		out << color.x << " " << color.y << " " << color.z;
		pugi::xml_node colorNode = componentNode.append_child("color");
		colorNode.append_attribute("val").set_value(out.str().c_str());
	}

	//////////////////////////////////////////////////////////

	void LightComponent::SetPosition(const math::Vector3f& position)
	{
		m_light->SetPosition(position);
		m_needsToRecalculate = true;
	}

	//////////////////////////////////////////////////////////

	const math::Vector3f& LightComponent::GetPosition() const
	{
		return m_light->GetPosition();
	}

	//////////////////////////////////////////////////////////

	void LightComponent::SetDirection(const math::Vector3f& direction)
	{
		m_light->SetDirection(direction);
		m_needsToRecalculate = true;
	}

	//////////////////////////////////////////////////////////

	const math::Vector3f& LightComponent::GetDirection() const
	{
		return m_light->GetDirection();
	}

	//////////////////////////////////////////////////////////

	void LightComponent::SetColor(math::Vector3f& color)
	{
		m_light->SetColor(color);
		m_needsToRecalculate = true;
	}

	//////////////////////////////////////////////////////////

	const math::Vector3f&  LightComponent::GetColor() const
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