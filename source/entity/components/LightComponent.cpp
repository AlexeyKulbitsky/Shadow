#include "LightComponent.h"
#include "../../scene/Light.h"

namespace sh
{
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

				pugi::xml_node directionNode = node.child("direction");  
				if (directionNode)
				{
					pugi::xml_attribute valAttr = directionNode.attribute("val");
					String directionString = valAttr.as_string();
					std::istringstream in(directionString);
					float val = 0.0f;
					std::vector<float> temp;
					while (in >> val) temp.push_back(val);
					math::Vector3f direction(0.0f);
					direction.x = temp[0];
					direction.y = temp[1];
					direction.z = temp[2];

					m_light->SetDirection(direction);
				}
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
}