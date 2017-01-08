#ifndef SHADOW_RENDER_COMPONENT_DECORATOR
#define SHADOW_RENDER_COMPONENT_DECORATOR

#include "ComponentDecorator.h"
#include <entity\components\RenderComponent.h>

class RenderComponentDecorator : public QObject, public sh::RenderComponent
{
	Q_OBJECT
public:
	ExpandableWidget* GetParametersWidget();

private:
	ExpandableWidget* m_parametersWidget = nullptr;
};

#endif