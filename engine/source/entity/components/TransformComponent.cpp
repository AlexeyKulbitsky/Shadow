#include "TransformComponent.h"
#include "../../serialization/ObjectFactory.h"

namespace sh
{
	TransformComponent::TransformComponent()
	{
		m_position = math::Vector3f(0.0f);
		m_rotation.SetIndentity();
		m_scale = math::Vector3f(1.0f);
		m_needsToRecalculateWorldMatrix = false;
	}

	/////////////////////////////////////////////////////////////////

	TransformComponent::~TransformComponent()
	{

	}

	/////////////////////////////////////////////////////////////////

	void TransformComponent::Load(const pugi::xml_node &node)
	{
		// Position
		pugi::xml_node positionNode = node.child("position");
		if (positionNode)
		{
			pugi::xml_attribute valAttr = positionNode.attribute("val");
			if (valAttr)
			{
				String positionString = valAttr.as_string();
				std::istringstream in(positionString);
				float val = 0.0f;
				std::vector<float> temp;
				while (in >> val) temp.push_back(val);
				math::Vector3f pos(0.0f);
				pos.x = temp[0];
				pos.y = temp[1];
				pos.z = temp[2];

				SetPosition(pos);
			}					
		}

		// Rotation
		pugi::xml_node rotationNode = node.child("rotation");
		if (rotationNode)
		{
			pugi::xml_attribute valAttr = rotationNode.attribute("val");
			if (valAttr)
			{			
				String rotationString = valAttr.as_string();
				std::istringstream in(rotationString);
				float val = 0.0f;
				std::vector<float> temp;
				while (in >> val) temp.push_back(val);
				math::Quaternionf rot;
				rot.x = temp[0];
				rot.y = temp[1];
				rot.z = temp[2];
				rot.w = temp[3];

				SetRotation(rot);
			}
		}

		// Scale
		pugi::xml_node scaleNodeNode = node.child("scale");
		if (scaleNodeNode)
		{
			pugi::xml_attribute valAttr = scaleNodeNode.attribute("val");
			if (valAttr)
			{
				String scaleString = valAttr.as_string();
				std::istringstream in(scaleString);
				float val = 0.0f;
				std::vector<float> temp;
				while (in >> val) temp.push_back(val);
				math::Vector3f scale(1.0f);
				scale.x = temp[0];
				scale.y = temp[1];
				scale.z = temp[2];

				SetScale(scale);
			}
		}
	}

	/////////////////////////////////////////////////////////////////

	void TransformComponent::Save(pugi::xml_node &parent)
	{
		pugi::xml_node componentNode = parent.append_child("component");
		componentNode.append_attribute("name").set_value("transform");

		pugi::xml_node posNode = componentNode.append_child("position");
		std::ostringstream outPos;
		outPos << m_position.x << " " << m_position.y << " " << m_position.z;		
		posNode.append_attribute("val").set_value(outPos.str().c_str());

		pugi::xml_node rotNode = componentNode.append_child("rotation");
		std::ostringstream outRot;
		outRot << m_rotation.x << " " << m_rotation.y << " " << m_rotation.z << " " << m_rotation.w;		
		rotNode.append_attribute("val").set_value(outRot.str().c_str());

		pugi::xml_node scaleNode = componentNode.append_child("scale");
		std::ostringstream outScale;
		outScale << m_scale.x << " " << m_scale.y << " " << m_scale.z;		
		scaleNode.append_attribute("val").set_value(outScale.str().c_str());
	}

	/////////////////////////////////////////////////////////////////

	void TransformComponent::RegisterObject()
	{
		ObjectFactory::GetInstance()->RegisterFactory<TransformComponent>();
		S_ACCESSOR_PROPERTY("Position", GetPosition, SetPosition);
	}

	/////////////////////////////////////////////////////////////////

}