#include "RenderComponentDecorator.h"

ExpandableWidget* RenderComponentDecorator::GetParametersWidget()
{
	m_parametersWidget = new ExpandableWidget("Render");

	return m_parametersWidget;
}

///////////////////////////////////////////////////