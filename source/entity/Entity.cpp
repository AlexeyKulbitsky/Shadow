#include "Entity.h"

namespace sh
{
	Entity::Entity()
	{

	}

	//////////////////////////////////////////////////////////////

	Entity::~Entity()
	{

	}

	//////////////////////////////////////////////////////////////

	void Entity::AddComponent(Component* component)
	{
		m_components.push_back(component);
	}

	//////////////////////////////////////////////////////////////

	Component* Entity::GetComponent(Component::Type type)
	{
		for (size_t i = 0, sz = m_components.size(); i < sz; ++i)
		{
			if (m_components[i]->GetType() == type)
				return m_components[i];
		}

		return nullptr;
	}

	//////////////////////////////////////////////////////////////
}

