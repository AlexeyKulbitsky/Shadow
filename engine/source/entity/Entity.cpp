#include "Entity.h"
#include "components\TransformComponent.h"
#include "components\RenderComponent.h"


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

	void Entity::Save(pugi::xml_node& parent)
	{
		pugi::xml_node entityNode = parent.append_child("entity");
		entityNode.append_attribute("name").set_value(m_name.c_str());

		for (size_t i = 0, sz = static_cast<size_t>( Component::Type::COUNT ); i < sz; ++i)
		{
			Component* component = m_components[i];
			if (component)
			{
				component->Save(entityNode);
			}
		}
	}

	//////////////////////////////////////////////////////////////

	void Entity::AddComponent(Component* component)
	{
		m_components[static_cast<size_t>(component->GetType())] = component;
		component->SetParentEntity(this);
	}

	//////////////////////////////////////////////////////////////

	void Entity::SetComponent(Component::Type type, Component* component)
	{
		m_components[static_cast<size_t>(type)] = component;
		component->SetParentEntity(this);
	}

	//////////////////////////////////////////////////////////////

	Component* Entity::GetComponent(Component::Type type)
	{		
		return m_components[static_cast<size_t>(type)];
	}

	//////////////////////////////////////////////////////////////

	bool Entity::IntersectsRay(const math::Vector3f& origin, const math::Vector3f& direction)
	{		
		if (GetComponent(Component::Type::TRANSFORM) && GetComponent(Component::Type::RENDER))
		{
			TransformComponent* transformComponent = static_cast<TransformComponent*>(GetComponent(Component::Type::TRANSFORM));
			RenderComponent* renderComponent = static_cast<RenderComponent*>(GetComponent(Component::Type::RENDER));

			math::Vector3f pos = transformComponent->GetPosition();

			float radius = 1.0f;
			float t1 = 0.0f, t2 = 0.0f;
			int result = math::RayIntersectSphere(origin, direction, pos, radius, t1, t2);
			if (result != 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		return false;
	}

	//////////////////////////////////////////////////////////////
}

