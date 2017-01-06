#include "EditorComponentsFactory.h"
#include "TransformComponentDecorator.h"

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
	}
	return sh::ComponentsFactory::CreateComponent(type);
}

///////////////////////////////////////////////////////////////////////////////