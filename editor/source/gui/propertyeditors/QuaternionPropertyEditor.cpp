#include "QuaternionPropertyEditor.h"

QuaternionPropertyEditor::QuaternionPropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
	: m_object(object)
	, m_objectProperty(objectProperty)
{
	SH_ASSERT(object, "Can not create QuaternionProperty editor for empty object");
	SH_ASSERT(objectProperty, "Can not create QuaternionProperty editor for empty property");
	SH_ASSERT(objectProperty->GetType() == sh::VAR_QUATERNION_FLOAT, "Can not create QuaternionProperty editor. Property type does not match");

	sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
	label->SetMaximumWidth(75U);
	mainLayout->AddWidget(label);
	m_edit.reset(new Vector3LineEdit());
	mainLayout->AddWidget(m_edit);
	SetLayout(mainLayout);
	SetMaximumHeight(20);

	const auto& quaternionRotation = objectProperty->GetValue(object).GetQuaternionFloat();
	sh::math::Vector3 eulerRotations;
	quaternionRotation.GetAsEulerXYZ(eulerRotations);

	m_edit->SetValue(sh::math::Degrees(eulerRotations));
	m_edit->valueChanged.Connect(std::bind(&QuaternionPropertyEditor::OnValueChanged, this, std::placeholders::_1));
}

void QuaternionPropertyEditor::OnValueChanged(const sh::math::Vector3& value)
{
	sh::math::Quaternion rot;
	rot.SetFromEulerXYZ(sh::math::Radians(value));

	m_objectProperty->SetValue(m_object, value);
}