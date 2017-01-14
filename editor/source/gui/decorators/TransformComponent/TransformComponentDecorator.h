#ifndef SHADOW_TRANSFORM_COMPONENT_DECORATOR_INCLUDE
#define SHADOW_TRANSFORM_COMPONENT_DECORATOR_INCLUDE

#include "ComponentDecorator.h"
#include <entity\components\TransformComponent.h>


class TransformComponentDecorator : public QObject, public sh::TransformComponent
{
	Q_OBJECT
public:
	virtual void SetPosition(const sh::math::Vector3f& position) override;
	virtual void SetRotation(const sh::math::Quaternionf& rotation) override;
	virtual void SetScale(const sh::math::Vector3f& scale) override;

	ExpandableWidget* GetParametersWidget();

signals:
	void PositionXChangedSignal(double value);
	void PositionYChangedSignal(double value);
	void PositionZChangedSignal(double value);

	void RotationXChangedSignal(double value);
	void RotationYChangedSignal(double value);
	void RotationZChangedSignal(double value);

	void ScaleXChangedSignal(double value);
	void ScaleYChangedSignal(double value);
	void ScaleZChangedSignal(double value);

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