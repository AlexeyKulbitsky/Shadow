#include "ScriptPropertyEditor.h"
#include "PropertyEditorsFactory.h"

ScriptPropertyEditor::ScriptPropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
	: m_object(object)
	, m_objectProperty(objectProperty)
{
	SH_ASSERT(object, "Can not create ScriptProperty editor for empty object");
	SH_ASSERT(objectProperty, "Can not create ScriptProperty editor for empty property");
	SH_ASSERT(objectProperty->GetType() == sh::VAR_SCRIPT_REF, "Can not create ScriptProperty editor. Property type does not match");

	sh::gui::HorizontalLayoutPtr scriptLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
	label->SetMaximumWidth(75U);
	scriptLayout->AddWidget(label);
	m_comboBox.reset(new sh::gui::ComboBox());
	scriptLayout->AddWidget(m_comboBox);
	sh::gui::WidgetPtr scriptWidget(new sh::gui::Widget());
	scriptWidget->SetMaximumHeight(20);
	scriptWidget->SetLayout(scriptLayout);

	sh::gui::VerticalLayoutPtr mainLayout(new sh::gui::VerticalLayout());

	sh::gui::WidgetPtr contentWidget(new sh::gui::Widget());
	m_contentLayout.reset(new sh::gui::VerticalLayout());
	m_contentLayout->SetSpacing(2);
	contentWidget->SetLayout(m_contentLayout);

	mainLayout->SetSpacing(5);
	mainLayout->AddWidget(scriptWidget);
	mainLayout->AddWidget(contentWidget);
	SetLayout(mainLayout);

	const auto& value = objectProperty->GetValue(object).GetScriptRef();
	auto scriptComponent = static_cast<sh::ScriptComponent*>(m_object);
	m_script = scriptComponent->GetScript();

	m_comboBox->AddItem("-None-");
	auto scripts = sh::ObjectFactory::GetInstance()->GetObjectTypesForGroup("Scripts");
	sh::u32 scriptIndex = 0U;
	for (size_t i = 0; i < scripts.size(); ++i)
	{
		m_comboBox->AddItem(scripts[i]);

		if (value.type == scripts[i])
			scriptIndex = i + 1;
	}
	m_comboBox->SetSelectedItem(scriptIndex);
	m_comboBox->OnItemChanged.Connect(std::bind(&ScriptPropertyEditor::OnScriptSelected, this, std::placeholders::_1));

	if (m_script)
	{
		UpdateScriptProperties();
	}
}

void ScriptPropertyEditor::OnScriptSelected(sh::u32 itemIndex)
{
	if (itemIndex == 0)
	{
		sh::ScriptRef ref;
		m_objectProperty->SetValue(m_object, ref);
		m_contentLayout->Clear();
		return;
	}

	auto scripts = sh::ObjectFactory::GetInstance()->GetObjectTypesForGroup("Scripts");
	sh::ScriptRef ref;
	ref.type = scripts[itemIndex - 1];
	m_objectProperty->SetValue(m_object, ref);

	auto scriptComponent = static_cast<sh::ScriptComponent*>(m_object);
	m_script = scriptComponent->GetScript();

	if (m_script)
	{
		UpdateScriptProperties();
	}
}

void ScriptPropertyEditor::UpdateScriptProperties()
{
	m_contentLayout->Clear();

	auto properties = sh::ObjectFactory::GetInstance()->GetProperties(m_script->GetTypeName());
	if (!properties)
		return;

	for (const auto& property : (*properties))
	{
		auto propertyEditor = PropertyEditorsFactory::GetInstance()->CreatePropertyEditor(m_script, property.second);
		if (propertyEditor)
			m_contentLayout->AddWidget(propertyEditor);
	}
}
