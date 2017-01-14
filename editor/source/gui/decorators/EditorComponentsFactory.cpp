#include "EditorComponentsFactory.h"
#include "TransformComponent/TransformComponentDecorator.h"
#include "RenderComponent/RenderComponentDecorator.h"

///////////////////////////////////////////////////////////////////////////////

EditorComponentsFactory::~EditorComponentsFactory()
{

}

///////////////////////////////////////////////////////////////////////////////

sh::Component* EditorComponentsFactory::CreateComponent(sh::Component::Type type)
{
	sh::Component* component = nullptr;
	switch (type)
	{
	case sh::Component::Type::TRANSFORM:
	{
		component = new TransformComponentDecorator();
		m_components.push_back(component);
		return component;
	}			
	case sh::Component::Type::RENDER:
	{
		component = new RenderComponentDecorator();
		m_components.push_back(component);
		return component;
	}
	}
	return sh::ComponentsFactory::CreateComponent(type);
}

///////////////////////////////////////////////////////////////////////////////