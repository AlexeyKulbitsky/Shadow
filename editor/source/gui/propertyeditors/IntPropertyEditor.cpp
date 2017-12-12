#include "IntPropertyEditor.h"


IntPropertyEditor::IntPropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
	: m_object(object)
	, m_objectProperty(objectProperty)
{
	SH_ASSERT(object, "Can not create IntProperty editor for empty object");
	SH_ASSERT(objectProperty, "Can not create IntProperty editor for empty property");
	SH_ASSERT(objectProperty->GetType() == sh::VAR_INT, "Can not create IntProperty editor. Property type does not match");

	sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
	label->SetMaximumWidth(75U);
	mainLayout->AddWidget(label);
	SetLayout(mainLayout);
	SetMaximumHeight(20);

	const auto value = objectProperty->GetValue(object).GetInt();
	const auto& enumNames = objectProperty->GetEnumNames();
	if (enumNames.size() == 0)
	{
		sh::gui::IntLineEditPtr intEdit(new sh::gui::IntLineEdit());
		mainLayout->AddWidget(intEdit);
		intEdit->SetValue(value);
		intEdit->OnValueChanged.Connect(std::bind(&IntPropertyEditor::OnValueChanged, this, std::placeholders::_1));
	}
	else
	{
		sh::gui::ComboBoxPtr comboBox(new sh::gui::ComboBox());
		for (const auto& enumName : enumNames)
		{
			comboBox->AddItem(enumName);
		}
		comboBox->SetSelectedItem(static_cast<sh::u32>(value));
		mainLayout->AddWidget(comboBox);
		comboBox->OnItemChanged.Connect(std::bind(&IntPropertyEditor::OnEnumValueChanged, this, std::placeholders::_1));
	}
}

void IntPropertyEditor::OnValueChanged(int value)
{
	m_objectProperty->SetValue(m_object, value);
}

void IntPropertyEditor::OnEnumValueChanged(sh::u32 value)
{
	m_objectProperty->SetValue(m_object, static_cast<int>(value));
}