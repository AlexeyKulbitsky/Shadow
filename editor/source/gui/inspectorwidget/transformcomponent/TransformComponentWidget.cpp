#include "TransformComponentWidget.h"

TransformWidget::TransformWidget()
{
	sh::gui::ButtonPtr button(new sh::gui::Button("Transform"));
	button->SetToggleable(true);
	button->OnToggle.Connect(std::bind(&TransformWidget::OnButtonToggled, this, 
		std::placeholders::_1));

	// Position part
	sh::gui::HorizontalLayoutPtr positionLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr positionLabel(new sh::gui::Label("Position"));
	positionLabel->SetMaximumWidth(75U);
	positionLayout->AddWidget(positionLabel);
	m_positionEdit.reset(new Vector3LineEdit());
	positionLayout->AddWidget(m_positionEdit);

	// Rotation part
	sh::gui::HorizontalLayoutPtr rotationLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr rotationLabel(new sh::gui::Label("Rotation"));
	rotationLabel->SetMaximumWidth(75U);
	rotationLayout->AddWidget(rotationLabel);
	m_rotationEdit.reset(new Vector3LineEdit());
	rotationLayout->AddWidget(m_rotationEdit);

	// Scale part
	sh::gui::HorizontalLayoutPtr scaleLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr scaleLabel(new sh::gui::Label("Scale"));
	scaleLabel->SetMaximumWidth(75U);
	scaleLayout->AddWidget(scaleLabel);
	m_scaleEdit.reset(new Vector3LineEdit());
	scaleLayout->AddWidget(m_scaleEdit);

	// Common layout
	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	layout->SetMargins(2, 2, 2, 2);
	layout->SetSpacing(2);
	layout->AddWidget(button);
	layout->AddLayout(positionLayout);
	layout->AddLayout(rotationLayout);
	layout->AddLayout(scaleLayout);

	m_widget.reset(new sh::gui::Widget());
	m_widget->SetMaximumHeight(80);
	m_widget->SetLayout(layout);


	//m_widget->SetMaximumHeight(75U);

	m_positionEdit->valueChanged.Connect(std::bind(&TransformWidget::OnPositinChanged, this, 
		std::placeholders::_1));
	m_rotationEdit->valueChanged.Connect(std::bind(&TransformWidget::OnRotationChanged, this,
		std::placeholders::_1));
	m_scaleEdit->valueChanged.Connect(std::bind(&TransformWidget::OnScaleChanged, this,
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
	
	Update();
}

void TransformWidget::Update()
{
	m_positionEdit->SetValue(m_transformComponent->GetPosition());
	sh::math::Vector3f eulerRotations;
	m_transformComponent->GetRotation().GetAsEulerXYZ(eulerRotations);
	m_rotationEdit->SetValue(sh::math::Degrees(eulerRotations));
	m_scaleEdit->SetValue(m_transformComponent->GetScale());
}

void TransformWidget::OnPositinChanged(const sh::math::Vector3f& position)
{
	if (!m_transformComponent)
		return;

	m_transformComponent->SetPosition(position);
}

void TransformWidget::OnRotationChanged(const sh::math::Vector3f& rotation)
{
	if (!m_transformComponent)
		return;

	sh::math::Quaternionf rot;
	rot.SetFromEulerXYZ(sh::math::Radians(rotation));
	m_transformComponent->SetRotation(rot);
}

void TransformWidget::OnScaleChanged(const sh::math::Vector3f& scale)
{
	if (!m_transformComponent)
		return;

	m_transformComponent->SetScale(scale);
}

void TransformWidget::OnButtonToggled(bool toggled)
{
	if (toggled)
	{

	}
	else
	{

	}
	m_widget->SetVisible(toggled);
}
