#include "ObjectFactory.h"

namespace sh
{

	Property* ObjectFactory::GetProperty(const std::string& objectTypeName, const std::string& name)
	{
		auto objectProperties = m_propertes.find(objectTypeName);
		if (objectProperties == m_propertes.end())
			return nullptr;

		auto property = objectProperties->second.find(name);
		if (property == objectProperties->second.end())
			return nullptr;

		return property->second;
	}

	std::map<std::string, Property*>* ObjectFactory::GetProperties(const std::string& objectTypeName)
	{
		auto objectProperties = m_propertes.find(objectTypeName);
		if (objectProperties == m_propertes.end())
			return nullptr;

		return &objectProperties->second;
	}

	Serializable* ObjectFactory::CreateObject(const std::string& type)
	{
		auto it = m_factories.find(type);
		if (it == m_factories.end())
			return nullptr;

		return it->second->CreateObject();
	}

	std::vector<std::string> ObjectFactory::GetObjectTypes() const
	{
		std::vector<std::string> result;

		for (auto factory : m_factories)
		{
			result.push_back(factory.first);
		}

		return result;
	}

	std::vector<std::string> ObjectFactory::GetObjectTypesForGroup(const String& groupName) const
	{
		std::vector<std::string> result;

		auto it = m_objectGroups.find(groupName);
		if (it != m_objectGroups.end())
			result = it->second;

		return result;
	}
    
    String ObjectFactory::GetGroupForObject(Serializable* object)
    {
        String typeName = object->GetTypeName();
        for (auto& it : m_objectGroups)
        {
            auto result = std::find(it.second.begin(), it.second.end(), typeName);
            if (result != it.second.end())
            {
                return it.first;
            }
        }
        return String();
    }

} // sh
