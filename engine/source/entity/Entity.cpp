#include "Entity.h"

namespace sh
{
	Entity::Entity()
	{
		for (size_t i = 0; i < static_cast<size_t>(Component::Type::COUNT); ++i)
		{
			m_components[i] = nullptr;
		}
	}

	//////////////////////////////////////////////////////////////

	Entity::~Entity()
	{

	}

	//////////////////////////////////////////////////////////////

	void Entity::AddComponent(Component* component)
	{
		m_components[static_cast<size_t>(component->GetType())] = component;
	}

	//////////////////////////////////////////////////////////////

	Component* Entity::GetComponent(Component::Type type)
	{		
		return m_components[static_cast<size_t>(type)];
	}

	//////////////////////////////////////////////////////////////
}

