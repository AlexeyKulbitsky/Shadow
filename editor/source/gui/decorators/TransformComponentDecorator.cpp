#include "TransformComponentDecorator.h"
#include <entity\components\TransformComponent.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QDoubleSpinBox>

/////////////////////////////////////////////////////////////////////////////////////////////////

ExpandableWidget* TransformComponentDecorator::GetParametersWidget()
{
	m_parametersWidget = new ExpandableWidget("Transform test");
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

	QDoubleSpinBox* yPositionSpinBox = new QDoubleSpinBox();
	yPositionSpinBox->setMinimum(-1000000.0);
	yPositionSpinBox->setMaximum(1000000.0);
	yPositionSpinBox->setValue(GetPosition().y);
	connect(yPositionSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetPositionYSlot(double)));

	QDoubleSpinBox* zPositionSpinBox = new QDoubleSpinBox();
	zPositionSpinBox->setMinimum(-1000000.0);
	zPositionSpinBox->setMaximum(1000000.0);
	zPositionSpinBox->setValue(GetPosition().z);
	connect(zPositionSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetPositionZSlot(double)));

	positionLayout->addWidget(positionLabel);
	positionLayout->addWidget(xPositionSpinBox);
	positionLayout->addWidget(yPositionSpinBox);
	positionLayout->addWidget(zPositionSpinBox);
	
	// Rotation
	QHBoxLayout* rotationLayout = new QHBoxLayout();
	QLabel* rotationLabel = new QLabel();
	rotationLabel->setText("Rotation");
	QDoubleSpinBox* xRotationSpinBox = new QDoubleSpinBox();
	QDoubleSpinBox* yRotationSpinBox = new QDoubleSpinBox();
	QDoubleSpinBox* zRotationSpinBox = new QDoubleSpinBox();
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

	QDoubleSpinBox* yScaleSpinBox = new QDoubleSpinBox();
	yScaleSpinBox->setMinimum(0.0);
	yScaleSpinBox->setMaximum(1000000.0);
	yScaleSpinBox->setValue(GetScale().y);
	connect(yScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetScaleYSlot(double)));

	QDoubleSpinBox* zScaleSpinBox = new QDoubleSpinBox();
	zScaleSpinBox->setMinimum(0.0);
	zScaleSpinBox->setMaximum(1000000.0);
	zScaleSpinBox->setValue(GetScale().z);
	connect(zScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetScaleZSlot(double)));

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

/////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetRotationXSlot(double value)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetRotationYSlot(double value)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

void TransformComponentDecorator::SetRotationZSlot(double value)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////

