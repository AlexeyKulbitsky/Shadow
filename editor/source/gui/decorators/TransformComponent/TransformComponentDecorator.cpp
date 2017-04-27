#include "TransformComponentDecorator.h"
#if 0
#include <entity\components\TransformComponent.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QDoubleSpinBox>

////////////////////////////////////////////////////////////////////

TransformComponentDecorator::~TransformComponentDecorator()
{

}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetPosition(const sh::math::Vector3f& position)
{
	TransformComponent::SetPosition(position);
	emit(PositionXChangedSignal((double)position.x));
	emit(PositionYChangedSignal((double)position.y));
	emit(PositionZChangedSignal((double)position.z));
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetRotation(const sh::math::Quaternionf& rotation)
{
	TransformComponent::SetRotation(rotation);
	sh::math::Vector3f eulerRot(0.0f);
	rotation.GetAsEulerXYZ(eulerRot);
	eulerRot *= 180.0 / sh::math::k_pi;
	//emit(RotationXChangedSignal((double)eulerRot.x));
	//emit(RotationYChangedSignal((double)eulerRot.y));
	//emit(RotationZChangedSignal((double)eulerRot.z));
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetScale(const sh::math::Vector3f& scale)
{
	TransformComponent::SetScale(scale);
	emit(ScaleXChangedSignal((double)scale.x));
	emit(ScaleYChangedSignal((double)scale.y));
	emit(ScaleZChangedSignal((double)scale.z));
}

////////////////////////////////////////////////////////////////////

ExpandableWidget* TransformComponentDecorator::GetParametersWidget()
{
	m_parametersWidget = new ExpandableWidget("Transform");
	QVBoxLayout* mainLayout = new QVBoxLayout();
	
	// Position
	QHBoxLayout* positionLayout = new QHBoxLayout();
	QLabel* positionLabel = new QLabel();
	positionLabel->setText("Position");
	QDoubleSpinBox* xPositionSpinBox = new QDoubleSpinBox();
	xPositionSpinBox->setMinimum(-1000000.0);
	xPositionSpinBox->setMaximum(1000000.0);
	xPositionSpinBox->setValue(GetPosition().x);
	connect(xPositionSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetPositionXSlot(double)));
	connect(this, SIGNAL(PositionXChangedSignal(double)), xPositionSpinBox, SLOT(setValue(double)));

	QDoubleSpinBox* yPositionSpinBox = new QDoubleSpinBox();
	yPositionSpinBox->setMinimum(-1000000.0);
	yPositionSpinBox->setMaximum(1000000.0);
	yPositionSpinBox->setValue(GetPosition().y);
	connect(yPositionSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetPositionYSlot(double)));
	connect(this, SIGNAL(PositionYChangedSignal(double)), yPositionSpinBox, SLOT(setValue(double)));

	QDoubleSpinBox* zPositionSpinBox = new QDoubleSpinBox();
	zPositionSpinBox->setMinimum(-1000000.0);
	zPositionSpinBox->setMaximum(1000000.0);
	zPositionSpinBox->setValue(GetPosition().z);
	connect(zPositionSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetPositionZSlot(double)));
	connect(this, SIGNAL(PositionZChangedSignal(double)), zPositionSpinBox, SLOT(setValue(double)));

	positionLayout->addWidget(positionLabel);
	positionLayout->addWidget(xPositionSpinBox);
	positionLayout->addWidget(yPositionSpinBox);
	positionLayout->addWidget(zPositionSpinBox);
	
	// Rotation
	QHBoxLayout* rotationLayout = new QHBoxLayout();
	QLabel* rotationLabel = new QLabel();
	rotationLabel->setText("Rotation");
	QDoubleSpinBox* xRotationSpinBox = new QDoubleSpinBox();
	xRotationSpinBox->setSingleStep(10.0);
	xRotationSpinBox->setMinimum(-10000.0);
	xRotationSpinBox->setMaximum(10000.0);
	xRotationSpinBox->setValue(m_axisRotations.x * 180.0f / sh::math::k_pi);
	connect(xRotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetRotationXSlot(double)));
	connect(this, SIGNAL(RotationXChangedSignal(double)), xRotationSpinBox, SLOT(setValue(double)));

	QDoubleSpinBox* yRotationSpinBox = new QDoubleSpinBox();
	yRotationSpinBox->setSingleStep(10.0);
	yRotationSpinBox->setMinimum(-10000.0);
	yRotationSpinBox->setMaximum(10000.0);
	yRotationSpinBox->setValue(m_axisRotations.y * 180.0f / sh::math::k_pi);
	connect(yRotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetRotationYSlot(double)));
	connect(this, SIGNAL(RotationYChangedSignal(double)), yRotationSpinBox, SLOT(setValue(double)));

	QDoubleSpinBox* zRotationSpinBox = new QDoubleSpinBox();
	zRotationSpinBox->setSingleStep(10.0);
	zRotationSpinBox->setMinimum(-10000.0);
	zRotationSpinBox->setMaximum(10000.0);
	zRotationSpinBox->setValue(m_axisRotations.z * 180.0f / sh::math::k_pi);
	connect(zRotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetRotationZSlot(double)));
	connect(this, SIGNAL(RotationZChangedSignal(double)), zRotationSpinBox, SLOT(setValue(double)));

	rotationLayout->addWidget(rotationLabel);
	rotationLayout->addWidget(xRotationSpinBox);
	rotationLayout->addWidget(yRotationSpinBox);
	rotationLayout->addWidget(zRotationSpinBox);

	// Scale
	QHBoxLayout* scaleLayout = new QHBoxLayout();
	QLabel* scaleLabel = new QLabel();
	scaleLabel->setText("Scale");
	QDoubleSpinBox* xScaleSpinBox = new QDoubleSpinBox();
	xScaleSpinBox->setMinimum(0.0);
	xScaleSpinBox->setMaximum(1000000.0);
	xScaleSpinBox->setValue(GetScale().x);
	connect(xScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetScaleXSlot(double)));
	connect(this, SIGNAL(ScaleXChangedSignal(double)), xScaleSpinBox, SLOT(setValue(double)));

	QDoubleSpinBox* yScaleSpinBox = new QDoubleSpinBox();
	yScaleSpinBox->setMinimum(0.0);
	yScaleSpinBox->setMaximum(1000000.0);
	yScaleSpinBox->setValue(GetScale().y);
	connect(yScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetScaleYSlot(double)));
	connect(this, SIGNAL(ScaleYChangedSignal(double)), yScaleSpinBox, SLOT(setValue(double)));

	QDoubleSpinBox* zScaleSpinBox = new QDoubleSpinBox();
	zScaleSpinBox->setMinimum(0.0);
	zScaleSpinBox->setMaximum(1000000.0);
	zScaleSpinBox->setValue(GetScale().z);
	connect(zScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetScaleZSlot(double)));
	connect(this, SIGNAL(ScaleZChangedSignal(double)), zScaleSpinBox, SLOT(setValue(double)));

	scaleLayout->addWidget(scaleLabel);
	scaleLayout->addWidget(xScaleSpinBox);
	scaleLayout->addWidget(yScaleSpinBox);
	scaleLayout->addWidget(zScaleSpinBox);

	mainLayout->addLayout(positionLayout);
	mainLayout->addLayout(rotationLayout);
	mainLayout->addLayout(scaleLayout);
	
	m_parametersWidget->setContentLayout(*mainLayout);
	
	return m_parametersWidget;
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetPositionXSlot(double value)
{
	sh::math::Vector3f currentPosition = GetPosition();
	double currentValue = (double)currentPosition.x;
	if (sh::math::Abs(value - currentValue) > sh::math::k_eps_7)
	{
		currentPosition.x = (float)value;
		SetPosition(currentPosition);
	}
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetPositionYSlot(double value)
{
	sh::math::Vector3f currentPosition = GetPosition();
	double currentValue = (double)currentPosition.y;
	if (sh::math::Abs(value - currentValue) > sh::math::k_eps_7)
	{
		currentPosition.y = (float)value;
		SetPosition(currentPosition);
	}
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetPositionZSlot(double value)
{
	sh::math::Vector3f currentPosition = GetPosition();
	double currentValue = (double)currentPosition.z;
	if (sh::math::Abs(value - currentValue) > sh::math::k_eps_7)
	{
		currentPosition.z = (float)value;
		SetPosition(currentPosition);
	}
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetRotationXSlot(double value)
{
	double currentValue = (double)m_axisRotations.x * 180.0 / sh::math::k_pi;
	if (sh::math::Abs(value - currentValue) > sh::math::k_eps_7)
	{
		m_axisRotations.x = (float)value * sh::math::k_pi / 180.0f;
		sh::math::Quaternionf rot;
		rot.SetFromEulerXYZ(m_axisRotations);
		SetRotation(rot);
	}
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetRotationYSlot(double value)
{
	double currentValue = (double)m_axisRotations.y * 180.0 / sh::math::k_pi;
	if (sh::math::Abs(value - currentValue) > sh::math::k_eps_7)
	{
		m_axisRotations.y = (float)value * sh::math::k_pi / 180.0f;
		sh::math::Quaternionf rot;
		rot.SetFromEulerXYZ(m_axisRotations);
		SetRotation(rot);
	}
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetRotationZSlot(double value)
{
	double currentValue = (double)m_axisRotations.z * 180.0 / sh::math::k_pi;
	if (sh::math::Abs(value - currentValue) > sh::math::k_eps_7)
	{
		m_axisRotations.z = (float)value * sh::math::k_pi / 180.0f;
		sh::math::Quaternionf rot;
		rot.SetFromEulerXYZ(m_axisRotations);
		SetRotation(rot);
	}
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetScaleXSlot(double value)
{
	sh::math::Vector3f currentScale = GetScale();
	double currentValue = (double)currentScale.x;
	if (sh::math::Abs(value - currentValue) > sh::math::k_eps_7)
	{
		currentScale.x = (float)value;
		SetScale(currentScale);
	}
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetScaleYSlot(double value)
{
	sh::math::Vector3f currentScale = GetScale();
	double currentValue = (double)currentScale.y;
	if (sh::math::Abs(value - currentValue) > sh::math::k_eps_7)
	{
		currentScale.y = (float)value;
		SetScale(currentScale);
	}
}

////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetScaleZSlot(double value)
{
	sh::math::Vector3f currentScale = GetScale();
	double currentValue = (double)currentScale.z;
	if (sh::math::Abs(value - currentValue) > sh::math::k_eps_7)
	{
		currentScale.z = (float)value;
		SetScale(currentScale);
	}
}

////////////////////////////////////////////////////////////////////

#endif
