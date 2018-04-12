#include "Entity.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "../serialization/ObjectFactory.h"
#include "../serialization/Serializer.h"
#include "../serialization/XMLSerializer.h"

#include "../Device.h"
#include "../scene/SceneManager.h"

#include <pugixml.hpp>

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

	Entity* Entity::Clone()
	{
		Entity* entity = new Entity();

		entity->m_components.resize(m_components.size());

		for (size_t i = 0; i < m_components.size(); ++i)
		{
			Component* component = nullptr;
			if (m_components[i])
			{
				component = m_components[i]->Clone();
				if (component)
					component->SetParentEntity(entity);
			}

			entity->m_components[i] = component;
		}

		return entity;
	}

	//////////////////////////////////////////////////////////////
	
	void Entity::Load(const pugi::xml_node& parent)
	{
		pugi::xml_attribute nameAttribute = parent.attribute("name");
		if (nameAttribute)
		{
			String name = nameAttribute.as_string();
			printf("Entity %s\n", name.c_str());
			SetName(name);
		}

		XMLSerializer serializer;

		pugi::xml_node componentNode = parent.child("component");

		while (componentNode)
		{
			pugi::xml_attribute componentType = componentNode.attribute("type");
			String typeStr = componentType.as_string();

			auto object = ObjectFactory::GetInstance()->CreateObject(typeStr);
			if (!object)
            {
                componentNode = componentNode.next_sibling();
                continue;
            }
				
			
			serializer.Deserialize(object, componentNode);

			Component* component = static_cast<Component*>(object);
			AddComponent(component);

			componentNode = componentNode.next_sibling();
		}
	}

	void Entity::Save(pugi::xml_node& parent)
	{
		pugi::xml_node entityNode = parent.append_child("entity");
		entityNode.append_attribute("name").set_value(m_name.c_str());

		XMLSerializer serializer;
		for (const auto& component : m_components)
		{
			if (component)
			{
				pugi::xml_node componentNode = entityNode.append_child("component");
				serializer.Serialize(component, componentNode);
				//component->Save(entityNode);
			}
		}
	}

	//////////////////////////////////////////////////////////////

	void Entity::AddComponent(Component* component)
	{
		const size_t id = component->GetId();
		if (id >= m_components.size())
			m_components.resize(id + 1, nullptr);
		m_components[id] = component;
		component->SetParentEntity(this);
	
		Device::GetInstance()->GetSceneManager()->RegisterEntity(this);
	}

	//////////////////////////////////////////////////////////////

	bool Entity::IntersectsRay(const math::Vector3& origin, const math::Vector3& direction)
	{		
		auto renderComponent = GetComponent<RenderComponent>();
		auto transformComponent = GetComponent<TransformComponent>();
		if (transformComponent && renderComponent)
		{
			math::Vector3 pos = transformComponent->GetPosition();

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

