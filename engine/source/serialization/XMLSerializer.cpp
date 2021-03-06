#include "XMLSerializer.h"
#include "Serializable.h"
#include "Property.h"
#include "ObjectFactory.h"
#include "../entity/components/ScriptComponent.h"
#include "../scripting/Script.h"

#include <pugixml.hpp>

namespace sh
{
    XMLSerializer::XMLSerializer(pugi::xml_node* root)
        : m_root(root)
        //, m_currentNode(root)
    {
        m_root = new pugi::xml_node;
        *m_root = *root;
    }
    
    XMLSerializer::~XMLSerializer()
    {
        if (m_root)
            delete m_root;
        m_root = nullptr;
    }
    
    void XMLSerializer::Serialize(Serializable* serializable)
    {
        //pugi::xml_node childNode = m_root->append_child("Object");
        //Serialize(serializable, childNode);
        Serialize(serializable, *m_root);
    }
    
    void XMLSerializer::Deserialize(Serializable* serializable)
    {
        
    }
    
    Serializer* XMLSerializer::Child()
    {
        pugi::xml_node childNode = m_root->append_child("Object");
        XMLSerializer* serializer = new XMLSerializer(&childNode);
        return serializer;
    }
    
	void XMLSerializer::Serialize(Serializable* serializable, pugi::xml_node& node)
	{
		// Set type of component
		node.append_attribute("type").set_value(serializable->GetTypeName());
        
        auto properties = ObjectFactory::GetInstance()->GetProperties(serializable->GetTypeName());
        if (!properties)
            return;

		for (auto prop : (*properties))
		{
			auto value = prop.second->GetValue(serializable);
			auto type = value.GetType();

			pugi::xml_node propertyNode = node.append_child(prop.second->GetName().c_str());

			switch (type)
			{
			case VAR_VECTOR_3_FLOAT:
			{
				auto vectorValue = value.GetVector3Float();
				std::ostringstream outValue;
				outValue << vectorValue.x << " " << vectorValue.y << " " << vectorValue.z;
				propertyNode.append_attribute("val").set_value(outValue.str().c_str());
			}
				break;
			case VAR_VECTOR_4_FLOAT:
			{
				auto vectorValue = value.GetVector4Float();
				std::ostringstream outValue;
				outValue << vectorValue.x << " " << vectorValue.y << " " << vectorValue.z << " " << vectorValue.w;
				propertyNode.append_attribute("val").set_value(outValue.str().c_str());
			}
				break;
			case VAR_COLOR:
			{
				auto vectorValue = value.GetColor();
				std::ostringstream outValue;
				outValue << vectorValue.red << " " << vectorValue.green << " " << vectorValue.blue << " " << vectorValue.alpha;
				propertyNode.append_attribute("val").set_value(outValue.str().c_str());
			}
				break;
			case VAR_QUATERNION_FLOAT:
			{
				auto quaternionValue = value.GetQuaternionFloat();
				std::ostringstream outValue;
				outValue << quaternionValue.x << " " << quaternionValue.y << " " << quaternionValue.z << " " << quaternionValue.w;
				propertyNode.append_attribute("val").set_value(outValue.str().c_str());
			}
				break;
			case VAR_INT:
			{
				propertyNode.append_attribute("val").set_value(value.GetInt());
			}
				break;
			case VAR_RESOURCE_REF:
			{
				auto resourceRef = value.GetResourceRef();
				propertyNode.append_attribute("val").set_value(resourceRef.name.c_str());
			}
				break;
			case VAR_NAMED_RESOURCE_REF_LIST:
			{
				const auto& namedResourceRefList = value.GetNamedResourceRefList();
				for (const auto& ref : namedResourceRefList)
				{
					pugi::xml_node refNode = propertyNode.append_child(ref.name.c_str());
					refNode.append_attribute("val").set_value(ref.resource.name.c_str());
				}
			}
				break;
			case VAR_SCRIPT_REF:
			{
				auto scriptComponent = static_cast<ScriptComponent*>(serializable);
				auto script = scriptComponent->GetScript();

				XMLSerializer scriptSerializer;
				scriptSerializer.Serialize(script, propertyNode);
			}
				break;
            case VAR_SERIALIZABLE:
            {
                auto s = value.GetSerializable();
                if (s)
                {
                    int a = 0;
                    a++;
                }
            }
                break;
            case VAR_SERIALIZABLE_SPTR:
            {
                auto serializablePtr = value.GetSerializableSPtr();
                Serialize(serializablePtr.get(), propertyNode);
            }
                break;
            case VAR_CUSTOM:
            {
                auto s = value.Get<Serializable*>();
                if (s)
                    Serialize(s, propertyNode);
            }
                break;
            case VAR_STRING:
            {
                propertyNode.append_attribute("val").set_value(value.GetString().c_str());
            }
                break;
            case VAR_BOOL:
            {
                propertyNode.append_attribute("val").set_value(value.GetBool());
            }
                break;
            case VAR_RECT:
            {
                auto rectValue = value.GetRect();
                std::ostringstream outValue;
                outValue << rectValue.lowerRightCorner.x << " " << rectValue.lowerRightCorner.y << " "
                        << rectValue.upperLeftCorner.x << " " << rectValue.upperLeftCorner.y;
                propertyNode.append_attribute("val").set_value(outValue.str().c_str());
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
				math::Vector3 vectorValue(0.0f);
				vectorValue.x = temp[0];
				vectorValue.y = temp[1];
				vectorValue.z = temp[2];

				prop.second->SetValue(serializable, vectorValue);
			}
			break;
			case VAR_VECTOR_4_FLOAT:
			{
				String valueString = propertyNode.attribute("val").as_string();

				std::istringstream in(valueString);
				float val = 0.0f;
				std::vector<float> temp;
				while (in >> val) temp.push_back(val);
				math::Vector4 vectorValue(0.0f);
				vectorValue.x = temp[0];
				vectorValue.y = temp[1];
				vectorValue.z = temp[2];
				vectorValue.w = temp[3];

				prop.second->SetValue(serializable, vectorValue);
			}
			break;
			case VAR_COLOR:
			{
				String valueString = propertyNode.attribute("val").as_string();

				std::istringstream in(valueString);
				float val = 0.0f;
				std::vector<float> temp;
				while (in >> val) temp.push_back(val);
				video::Color colorValue;
				colorValue.red = temp[0];
				colorValue.green = temp[1];
				colorValue.blue = temp[2];
				colorValue.alpha = temp[3];

				prop.second->SetValue(serializable, colorValue);
			}
			break;
			case VAR_QUATERNION_FLOAT:
			{
				String valueString = propertyNode.attribute("val").as_string();
				std::istringstream in(valueString);
				float val = 0.0f;
				std::vector<float> temp;
				while (in >> val) temp.push_back(val);
				math::Quaternion quaternionValue;
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
			case VAR_SCRIPT_REF:
			{
				auto scriptComponent = static_cast<ScriptComponent*>(serializable);

				String scriptType = propertyNode.attribute("type").as_string();
				auto script = ObjectFactory::GetInstance()->CreateObject(scriptType);

				if (!script)
					continue;

				XMLSerializer scriptSerializer;
				scriptSerializer.Deserialize(script, propertyNode);

				scriptComponent->SetScript(static_cast<Script*>(script));
			}
			break;
			default:
				break;
			}
		}
	}

} // sh
