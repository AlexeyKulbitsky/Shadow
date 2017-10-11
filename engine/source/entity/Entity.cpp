#include "Entity.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"


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

	void Entity::Save(pugi::xml_node& parent)
	{
		/*pugi::xml_node entityNode = parent.append_child("entity");
		entityNode.append_attribute("name").set_value(m_name.c_str());

		for (size_t i = 0, sz = static_cast<size_t>( Component::Type::Count ); i < sz; ++i)
		{
			Component* component = m_components[i];
			if (component)
			{
				component->Save(entityNode);
			}
		}*/
	}

	//////////////////////////////////////////////////////////////

	void Entity::AddComponent(Component* component)
	{
		const size_t id = component->GetId();
		if (id >= m_components.size())
			m_components.resize(id + 1, nullptr);
		m_components[id] = component;
		component->SetParentEntity(this);
	}

	//////////////////////////////////////////////////////////////

	bool Entity::IntersectsRay(const math::Vector3f& origin, const math::Vector3f& direction)
	{		
		auto renderComponent = GetComponent<RenderComponent>();
		auto transformComponent = GetComponent<TransformComponent>();
		if (transformComponent && renderComponent)
		{
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

