#include "XMLSerializer.h"
#include "Serializable.h"
#include "Property.h"
#include "ObjectFactory.h"

namespace sh
{

	void XMLSerializer::Serialize(Serializable* serializable, pugi::xml_node& node)
	{
		auto properties = ObjectFactory::GetInstance()->GetProperties(serializable->GetTypeName());
		if (!properties)
			return;

		// Set type of component
		node.append_attribute("type").set_value(serializable->GetTypeName());

		for (auto prop : (*properties))
		{
			auto value = prop.second->GetValue(serializable);
			auto type = value.GetType();

			pugi::xml_node porpertyNode = node.append_child(prop.second->GetName().c_str());

			switch (type)
			{
			case VAR_VECTOR_3_FLOAT:
			{
				auto vectorValue = value.GetVector3Float();
				std::ostringstream outValue;
				outValue << vectorValue.x << " " << vectorValue.y << " " << vectorValue.z;
				porpertyNode.append_attribute("val").set_value(outValue.str().c_str());
			}
				break;
			case VAR_QUATERNION_FLOAT:
			{
				auto quaternionValue = value.GetQuaternionFloat();
				std::ostringstream outValue;
				outValue << quaternionValue.x << " " << quaternionValue.y << " " << quaternionValue.z << " " << quaternionValue.w;
				porpertyNode.append_attribute("val").set_value(outValue.str().c_str());
			}
				break;
			case VAR_INT:
			{
				porpertyNode.append_attribute("val").set_value(value.GetInt());
			}
				break;
			case VAR_RESOURCE_REF:
			{
				auto resourceRef = value.GetResourceRef();
				porpertyNode.append_attribute("val").set_value(resourceRef.name.c_str());
			}
				break;
			case VAR_NAMED_RESOURCE_REF_LIST:
			{
				const auto& namedResourceRefList = value.GetNamedResourceRefList();
				for (const auto& ref : namedResourceRefList)
				{
					pugi::xml_node refNode = porpertyNode.append_child(ref.name.c_str());
					refNode.append_attribute("val").set_value(ref.resource.name.c_str());
				}
			}
				break;
			default:
				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void XMLSerializer::Deserialize(Serializable* serializable, const pugi::xml_node& node)
	{
		auto properties = ObjectFactory::GetInstance()->GetProperties(serializable->GetTypeName());
		if (!properties)
			return;

		for (auto prop : (*properties))
		{
			pugi::xml_node propertyNode = node.child(prop.first.c_str());
			if (!propertyNode)
				continue;

			auto type = prop.second->GetType();

			switch (type)
			{
			case VAR_VECTOR_3_FLOAT:
			{
				String valueString = propertyNode.attribute("val").as_string();

				std::istringstream in(valueString);
				float val = 0.0f;
				std::vector<float> temp;
				while (in >> val) temp.push_back(val);
				math::Vector3f vectorValue(0.0f);
				vectorValue.x = temp[0];
				vectorValue.y = temp[1];
				vectorValue.z = temp[2];

				prop.second->SetValue(serializable, vectorValue);
			}
			break;
			case VAR_QUATERNION_FLOAT:
			{
				String valueString = propertyNode.attribute("val").as_string();
				std::istringstream in(valueString);
				float val = 0.0f;
				std::vector<float> temp;
				while (in >> val) temp.push_back(val);
				math::Quaternionf quaternionValue;
				quaternionValue.x = temp[0];
				quaternionValue.y = temp[1];
				quaternionValue.z = temp[2];
				quaternionValue.w = temp[3];

				prop.second->SetValue(serializable, quaternionValue);
			}
			break;
			case VAR_INT:
			{
				const int intValue = propertyNode.attribute("val").as_int();

				prop.second->SetValue(serializable, intValue);
			}
			break;
			case VAR_RESOURCE_REF:
			{
				ResourceRef refValue;
				refValue.name = propertyNode.attribute("val").as_string();
				
				prop.second->SetValue(serializable, refValue);
			}
			break;
			case VAR_NAMED_RESOURCE_REF_LIST:
			{
				NamedResourceRefList refList;
				auto child = propertyNode.first_child();
				while (child)
				{
					NamedResourceRef ref;
					ref.name = child.name();
					ref.resource.name = child.attribute("val").as_string();
					refList.push_back(ref);

					child = child.next_sibling();
				}

				prop.second->SetValue(serializable, refList);
			}
			break;
			default:
				break;
			}
		}
	}

} // sh