#include "Vector3PropertyEditor.h"


Vector3PropertyEditor::Vector3PropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
	: m_object(object)
	, m_objectProperty(objectProperty)
{
	SH_ASSERT(object, "Can not create Vector3Property editor for empty object");
	SH_ASSERT(objectProperty, "Can not create Vector3Property editor for empty property");
	SH_ASSERT(objectProperty->GetType() == sh::VAR_VECTOR_3_FLOAT, "Can not create Vector3Property editor. Property type does not match");

	sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
	label->SetMaximumWidth(75U);
	mainLayout->AddWidget(label);
	m_edit.reset(new Vector3LineEdit());
	mainLayout->AddWidget(m_edit);

	const auto& value = objectProperty->GetValue(object).GetVector3Float();
	m_edit->SetValue(value);
	m_edit->valueChanged.Connect(std::bind(&Vector3PropertyEditor::OnValueChanged, this, std::placeholders::_1));
}

void Vector3PropertyEditor::OnValueChanged(const sh::math::Vector3f& value)
{
	m_objectProperty->SetValue(m_object, value);
}