#ifndef SHADOW_TRANSFORM_COMPONENT_DECORATOR_INCLUDE
#define SHADOW_TRANSFORM_COMPONENT_DECORATOR_INCLUDE

#include "ComponentDecorator.h"

class TransformComponenetDecorator : public ComponentDecorator
{
public:
	virtual ExpandableWidget* GetParametersWidget() override;
};

#endif