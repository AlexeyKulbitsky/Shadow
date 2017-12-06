#include "ScriptPropertyEditor.h"

ScriptPropertyEditor::ScriptPropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
	: m_object(object)
	, m_objectProperty(objectProperty)
{
	SH_ASSERT(object, "Can not create ScriptProperty editor for empty object");
	SH_ASSERT(objectProperty, "Can not create ScriptProperty editor for empty property");
	SH_ASSERT(objectProperty->GetType() == sh::VAR_SCRIPT_REF, "Can not create ScriptProperty editor. Property type does not match");

	sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
	label->SetMaximumWidth(75U);
	mainLayout->AddWidget(label);
	m_comboBox.reset(new sh::gui::ComboBox());
	mainLayout->AddWidget(m_comboBox);
	SetLayout(mainLayout);
	SetMaximumHeight(20);

	auto value = objectProperty->GetValue(object);
	const auto& resourceRef = value.GetResourceRef();
	auto scripts = sh::ObjectFactory::GetInstance()->GetObjectTypesForGroup("Scripts");
	for (size_t i = 0; i < scripts.size(); ++i)
	{
		m_comboBox->AddItem(scripts[i]);
	}
}
