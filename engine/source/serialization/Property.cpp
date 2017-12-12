#include "Property.h"

namespace sh
{

	const std::string& Property::GetName() const
	{
		return m_name;
	}

	const std::vector<String>& Property::GetEnumNames() const
	{
		return m_enumNames;
	}

} // sh

