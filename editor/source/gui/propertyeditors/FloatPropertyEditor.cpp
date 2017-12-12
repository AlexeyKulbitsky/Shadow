#include "FloatPropertyEditor.h"


FloatPropertyEditor::FloatPropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
	: m_object(object)
	, m_objectProperty(objectProperty)
{
	SH_ASSERT(object, "Can not create FloatProperty editor for empty object");
	SH_ASSERT(objectProperty, "Can not create FloatProperty editor for empty property");
	SH_ASSERT(objectProperty->GetType() == sh::VAR_FLOAT, "Can not create FloatProperty editor. Property type does not match");

	sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
	label->SetMaximumWidth(75U);
	mainLayout->AddWidget(label);
	SetLayout(mainLayout);
	SetMaximumHeight(20);

	const auto value = objectProperty->GetValue(object).GetFloat();
	sh::gui::FloatLineEditPtr intEdit(new sh::gui::FloatLineEdit());
	mainLayout->AddWidget(intEdit);
	intEdit->SetValue(value);
	intEdit->OnValueChanged.Connect(std::bind(&FloatPropertyEditor::OnValueChanged, this, std::placeholders::_1));
}

void FloatPropertyEditor::OnValueChanged(float value)
{
	m_objectProperty->SetValue(m_object, value);
}

