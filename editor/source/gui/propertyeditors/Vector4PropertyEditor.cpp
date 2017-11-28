#include "Vector4PropertyEditor.h"


Vector4PropertyEditor::Vector4PropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
	: m_object(object)
	, m_objectProperty(objectProperty)
{
	SH_ASSERT(object, "Can not create Vector4Property editor for empty object");
	SH_ASSERT(objectProperty, "Can not create Vector4Property editor for empty property");
	SH_ASSERT(objectProperty->GetType() == sh::VAR_VECTOR_4_FLOAT, "Can not create Vector4Property editor. Property type does not match");

	sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
	label->SetMaximumWidth(75U);
	mainLayout->AddWidget(label);
	m_edit.reset(new Vector4LineEdit());
	mainLayout->AddWidget(m_edit);
	SetLayout(mainLayout);
	SetMaximumHeight(20);

	const auto& value = objectProperty->GetValue(object).GetVector4Float();
	m_edit->SetValue(value);
	m_edit->valueChanged.Connect(std::bind(&Vector4PropertyEditor::OnValueChanged, this, std::placeholders::_1));
}

void Vector4PropertyEditor::OnValueChanged(const sh::math::Vector4f& value)
{
	m_objectProperty->SetValue(m_object, value);
}