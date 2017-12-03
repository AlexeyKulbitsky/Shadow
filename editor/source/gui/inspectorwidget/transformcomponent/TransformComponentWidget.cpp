#include "TransformComponentWidget.h"

TransformComponentWidget::TransformComponentWidget()
	: ExpandableWidget("Transform")
{
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

	m_contentsLayout->AddLayout(positionLayout);
	m_contentsLayout->AddLayout(rotationLayout);
	m_contentsLayout->AddLayout(scaleLayout);
	SetMaximumHeight(80);

	m_positionEdit->valueChanged.Connect(std::bind(&TransformComponentWidget::OnPositinChanged, this,
		std::placeholders::_1));
	m_rotationEdit->valueChanged.Connect(std::bind(&TransformComponentWidget::OnRotationChanged, this,
		std::placeholders::_1));
	m_scaleEdit->valueChanged.Connect(std::bind(&TransformComponentWidget::OnScaleChanged, this,
		std::placeholders::_1));
}

TransformComponentWidget::~TransformComponentWidget()
{

}

void TransformComponentWidget::SetTransformComponent(sh::TransformComponent* component)
{
	m_transformComponent = component;

	if (!component)
	{
		return;
	}
	
	Update();
}

void TransformComponentWidget::Update()
{
	m_positionEdit->SetValue(m_transformComponent->GetPosition());
	sh::math::Vector3 eulerRotations;
	m_transformComponent->GetRotation().GetAsEulerXYZ(eulerRotations);
	m_rotationEdit->SetValue(sh::math::Degrees(eulerRotations));
	m_scaleEdit->SetValue(m_transformComponent->GetScale());
}

void TransformComponentWidget::OnPositinChanged(const sh::math::Vector3& position)
{
	if (!m_transformComponent)
		return;

	m_transformComponent->SetPosition(position);
}

void TransformComponentWidget::OnRotationChanged(const sh::math::Vector3& rotation)
{
	if (!m_transformComponent)
		return;

	sh::math::Quaternion rot;
	rot.SetFromEulerXYZ(sh::math::Radians(rotation));
	m_transformComponent->SetRotation(rot);
}

void TransformComponentWidget::OnScaleChanged(const sh::math::Vector3& scale)
{
	if (!m_transformComponent)
		return;

	m_transformComponent->SetScale(scale);
}

