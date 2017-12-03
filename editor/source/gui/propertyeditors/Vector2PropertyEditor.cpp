#include "Vector2PropertyEditor.h"


Vector2PropertyEditor::Vector2PropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
	: m_object(object)
	, m_objectProperty(objectProperty)
{
	SH_ASSERT(object, "Can not create Vector2Property editor for empty object");
	SH_ASSERT(objectProperty, "Can not create Vector2Property editor for empty property");
	SH_ASSERT(objectProperty->GetType() == sh::VAR_VECTOR_2_FLOAT, "Can not create Vector2Property editor. Property type does not match");

	sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
	label->SetMaximumWidth(75U);
	mainLayout->AddWidget(label);
	m_edit.reset(new Vector2LineEdit());
	mainLayout->AddWidget(m_edit);
	SetLayout(mainLayout);
	SetMaximumHeight(20);

	const auto& value = objectProperty->GetValue(object).GetVector2Float();
	m_edit->SetValue(value);
	m_edit->valueChanged.Connect(std::bind(&Vector2PropertyEditor::OnValueChanged, this, std::placeholders::_1));
}

void Vector2PropertyEditor::OnValueChanged(const sh::math::Vector2& value)
{
	m_objectProperty->SetValue(m_object, value);
}