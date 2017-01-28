#ifndef SHADOW_RENDER_COMPONENT_DECORATOR
#define SHADOW_RENDER_COMPONENT_DECORATOR

#include "ComponentDecorator.h"
#include <Shadow.h>

class RenderComponentDecorator : public QObject, public sh::RenderComponent
{
	Q_OBJECT
public:
	RenderComponentDecorator();
	virtual ~RenderComponentDecorator();
	ExpandableWidget* GetParametersWidget();

private:
	ExpandableWidget* m_parametersWidget = nullptr;
};

#endif