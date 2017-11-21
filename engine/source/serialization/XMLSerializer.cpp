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

} // sh