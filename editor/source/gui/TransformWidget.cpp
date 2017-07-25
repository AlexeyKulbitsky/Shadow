#include "TransformWidget.h"


TransformWidget::TransformWidget()
{
	// Position part
	sh::gui::HorizontalLayoutPtr positionLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr positionLabel(new sh::gui::Label("Position"));
	sh::gui::FloatLineEditPtr xCoordEdit(new sh::gui::FloatLineEdit());
	sh::gui::FloatLineEditPtr yCoordEdit(new sh::gui::FloatLineEdit());
	sh::gui::FloatLineEditPtr zCoordEdit(new sh::gui::FloatLineEdit());

	sh::gui::ButtonPtr button(new sh::gui::Button());

	positionLayout->AddWidget(button);
	positionLayout->AddWidget(xCoordEdit);
	positionLayout->AddWidget(yCoordEdit);
	positionLayout->AddWidget(zCoordEdit);

	// Rotation part
	sh::gui::HorizontalLayoutPtr rotationLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr rotationLabel(new sh::gui::Label("Rotation"));
	sh::gui::FloatLineEditPtr xRotEdit(new sh::gui::FloatLineEdit());
	sh::gui::FloatLineEditPtr yRotEdit(new sh::gui::FloatLineEdit());
	sh::gui::FloatLineEditPtr zRotEdit(new sh::gui::FloatLineEdit());
	rotationLayout->AddWidget(rotationLabel);
	rotationLayout->AddWidget(xRotEdit);
	rotationLayout->AddWidget(yRotEdit);
	rotationLayout->AddWidget(zRotEdit);

	// Scale part
	sh::gui::HorizontalLayoutPtr scaleLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr scaleLabel(new sh::gui::Label("Scale"));
	sh::gui::FloatLineEditPtr xScaleEdit(new sh::gui::FloatLineEdit());
	sh::gui::FloatLineEditPtr yScaleEdit(new sh::gui::FloatLineEdit());
	sh::gui::FloatLineEditPtr zScaleEdit(new sh::gui::FloatLineEdit());
	scaleLayout->AddWidget(scaleLabel);
	scaleLayout->AddWidget(xScaleEdit);
	scaleLayout->AddWidget(yScaleEdit);
	scaleLayout->AddWidget(zScaleEdit);

	// Common layout
	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	layout->AddLayout(positionLayout);
	//layout->AddLayout(rotationLayout);
	//layout->AddLayout(scaleLayout);

	m_widget.reset(new sh::gui::Widget());
	m_widget->SetLayout(layout);
}

TransformWidget::~TransformWidget()
{

}