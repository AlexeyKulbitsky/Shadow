#include "System.h"
#include "Entity.h"

namespace sh
{
	void System::RegisterEntity(Entity* entity)
	{
		/*for (auto componentType : m_componentTypes)
		{
			if (entity->GetComponent(componentType) != nullptr)
			{
				AddEntity(entity);
				return;
			}
		}*/
	}

	void System::RegisterComponent(Component* component)
	{
	}

	void System::AddEntity(Entity* entity)
	{

	}
	void System::RemoveEntity(Entity* entity)
	{

	}

	void System::Update(f32 deltaTime)
	{

	}

	/*void System::AddComponentType(Component::Type type)
	{
		for (auto componentType : m_componentTypes)
		{
			if (componentType == type)
			{
				return;
			}
		}

		m_componentTypes.push_back(type);
	}*/

	void System::Clear()
	{
	}
}