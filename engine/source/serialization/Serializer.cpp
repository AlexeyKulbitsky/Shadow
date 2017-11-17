#include "Serializer.h"
#include "Serializable.h"
#include "Property.h"
#include "ObjectFactory.h"

#include <iostream>

namespace sh
{

	void Serializer::Serialize(Serializable* serializable)
	{
		auto properties = ObjectFactory::GetInstance()->GetProperties(serializable->GetTypeName());
		if (!properties)
			return;

		std::cout << "\nSerializing object: Type: " << serializable->GetTypeName() << std::endl;

		for (auto prop : (*properties))
		{
			auto value = prop.second->GetValue(serializable);
			auto type = value.GetType();

			std::cout << "Property [ " << prop.second->GetName() << " ]" << std::endl;
			switch (type)
			{
			case VAR_NOTYPE:
			{

			}
			break;
			case VAR_INT:
			{
				std::cout << "\tType: INT" << std::endl;
				std::cout << "\tValue: " << value.GetInt() << std::endl;
			}
			break;
			case VAR_FLOAT:
			{
				std::cout << "\tType: FLOAT" << std::endl;
				std::cout << "\tValue: " << value.GetFloat() << std::endl;
			}
			break;
			case VAR_BOOL:
			{
				std::cout << "\tType: BOOL" << std::endl;
				std::cout << "\tValue: " << value.GetBool() << std::endl;
			}
			break;
			case VAR_STRING:
			{
				std::cout << "\tType: STRING" << std::endl;
				std::cout << "\tValue: " << value.GetString() << std::endl;
			}
			break;
			case VAR_SERIALIZABLE:
			{
				std::cout << "\tType: SERIALIZABLE" << std::endl;
				std::cout << "\tValue: " << value.GetSerializable()->GetTypeName() << std::endl;
			}
			break;
			default:
				break;
			}
		}
	}

} // sh

