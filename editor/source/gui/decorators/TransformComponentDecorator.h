#ifndef SHADOW_TRANSFORM_COMPONENT_DECORATOR_INCLUDE
#define SHADOW_TRANSFORM_COMPONENT_DECORATOR_INCLUDE

#include "ComponentDecorator.h"
#include <entity\components\TransformComponent.h>


class TransformComponentDecorator : public QObject, public sh::TransformComponent
{
	Q_OBJECT
public:
	ExpandableWidget* GetParametersWidget();

public slots:
	void SetPositionXSlot(double value);
	void SetPositionYSlot(double value);
	void SetPositionZSlot(double value);

	void SetRotationXSlot(double value);
	void SetRotationYSlot(double value);
	void SetRotationZSlot(double value);

	void SetScaleXSlot(double value);
	void SetScaleYSlot(double value);
	void SetScaleZSlot(double value);


private:
	ExpandableWidget* m_parametersWidget = nullptr;
	sh::math::Vector3f m_axisRotations;
};

#endif