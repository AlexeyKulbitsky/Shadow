#ifndef SHADOW_EDITOR_COMPONENT_DECORATOR_INCLUDE
#define SHADOW_EDITOR_COMPONENT_DECORATOR_INCLUDE

#include <Shadow.h>

#include "../ExpandableWidget.h"

class ComponentDecorator : public sh::Component
{
public:
	virtual ExpandableWidget* GetParametersWidget() = 0;


	virtual Type GetType() const { return sh::Component::Type::COUNT; }
	virtual void Load(const pugi::xml_node &node) {}

protected:
	ExpandableWidget* m_parametersWidget = nullptr;
};

#endif