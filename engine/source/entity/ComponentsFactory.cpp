#include "ComponentsFactory.h"
#include "components/TransformComponent.h"
#include "components/RenderComponent.h"
#include "components/LightComponent.h"

namespace sh
{
	//////////////////////////////////////////////////////////////////////////////////////////

	ComponentsFactory::~ComponentsFactory()
	{
		for (size_t i = 0; i < m_components.size(); ++i)
		{
			delete m_components[i];
			m_components[i] = nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////

	Component* ComponentsFactory::CreateComponent(Component::Type type)
	{
		Component* component = nullptr;

		switch (type)
		{
		case Component::Type::TRANSFORM:
			component = new TransformComponent();
			break;
		case Component::Type::RENDER:
			component = new RenderComponent();
			break;
		case Component::Type::LIGHT:
			component = new LightComponent();
			break;
		}
		m_components.push_back(component);

		return component;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
}