#include "TransformWidget.h"

TransformWidget::TransformWidget()
{
	// Position part
	sh::gui::HorizontalLayoutPtr positionLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr positionLabel(new sh::gui::Label("Position"));
	positionLayout->AddWidget(positionLabel);
	positionLayout->AddWidget(m_positionEdit.GetWidget());

	// Rotation part
	sh::gui::HorizontalLayoutPtr rotationLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr rotationLabel(new sh::gui::Label("Rotation"));
	rotationLayout->AddWidget(rotationLabel);
	rotationLayout->AddWidget(m_rotationEdit.GetWidget());

	// Scale part
	sh::gui::HorizontalLayoutPtr scaleLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr scaleLabel(new sh::gui::Label("Scale"));
	scaleLayout->AddWidget(scaleLabel);
	scaleLayout->AddWidget(m_scaleEdit.GetWidget());

	// Common layout
	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	layout->AddLayout(positionLayout);
	layout->AddLayout(rotationLayout);
	layout->AddLayout(scaleLayout);

	m_widget.reset(new sh::gui::Widget());
	m_widget->SetLayout(layout);

	m_positionEdit.valueChanged.Connect(std::bind(&TransformWidget::OnPositinChanged, this, 
		std::placeholders::_1));
	m_rotationEdit.valueChanged.Connect(std::bind(&TransformWidget::OnRotationChanged, this,
		std::placeholders::_1));
	m_scaleEdit.valueChanged.Connect(std::bind(&TransformWidget::OnScaleChanged, this,
		std::placeholders::_1));
}

TransformWidget::~TransformWidget()
{

}

void TransformWidget::SetTransformComponent(sh::TransformComponent* component)
{
	m_transformComponent = component;

	if (!component)
	{
		return;
	}
	
	m_positionEdit.SetValue(m_transformComponent->GetPosition());
}

void TransformWidget::Update()
{
	m_positionEdit.SetValue(m_transformComponent->GetPosition());
}

void TransformWidget::OnPositinChanged(const sh::math::Vector3f& position)
{
	if (!m_transformComponent)
		return;

	m_transformComponent->SetPosition(position);
}

void TransformWidget::OnRotationChanged(const sh::math::Vector3f& rotation)
{
}

void TransformWidget::OnScaleChanged(const sh::math::Vector3f& scale)
{
}
