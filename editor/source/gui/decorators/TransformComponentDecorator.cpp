#include "TransformComponentDecorator.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDoubleSpinBox>

ExpandableWidget* TransformComponenetDecorator::GetParametersWidget()
{
	m_parametersWidget = new ExpandableWidget("Transform test");
	QVBoxLayout* mainLayout = new QVBoxLayout();
	
	// Position
	QHBoxLayout* positionLayout = new QHBoxLayout();
	QLabel* positionLabel = new QLabel();
	positionLabel->setText("Position");
	QDoubleSpinBox* xPositionSpinBox = new QDoubleSpinBox();
	QDoubleSpinBox* yPositionSpinBox = new QDoubleSpinBox();
	QDoubleSpinBox* zPositionSpinBox = new QDoubleSpinBox();
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
	QDoubleSpinBox* yScaleSpinBox = new QDoubleSpinBox();
	QDoubleSpinBox* zScaleSpinBox = new QDoubleSpinBox();
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

