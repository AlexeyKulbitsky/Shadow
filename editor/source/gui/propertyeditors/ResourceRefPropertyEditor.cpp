#include "ResourceRefPropertyEditor.h"

ResourceRefPropertyEditor::ResourceRefPropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
	: m_object(object)
	, m_objectProperty(objectProperty)
{
	SH_ASSERT(object, "Can not create ResourceRefProperty editor for empty object");
	SH_ASSERT(objectProperty, "Can not create ResourceRefProperty editor for empty property");
	SH_ASSERT(objectProperty->GetType() == sh::VAR_RESOURCE_REF, "Can not create ResourceRefProperty editor. Property type does not match");

	sh::gui::HorizontalLayoutPtr mainLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(objectProperty->GetName()));
	label->SetMaximumWidth(75U);
	mainLayout->AddWidget(label);

	m_comboBox.reset(new sh::gui::ComboBox());

	auto value = objectProperty->GetValue(object);
	const auto& resourceRef = value.GetResourceRef();
	auto resourceGroup = sh::ResourceManager::GetInstance()->GetResourceGroup(resourceRef.type);
	size_t index = 0;
	for (size_t i = 0; i < resourceGroup.size(); ++i)
	{
		m_comboBox->AddItem(resourceGroup[i].name);
		if (resourceGroup[i].name == resourceRef.name)
			index = i;
	}
	m_comboBox->SetSelectedItem(index);
 	m_comboBox->OnItemChanged.Connect(std::bind(&ResourceRefPropertyEditor::OnRefChanged, this,	std::placeholders::_1));
}

void ResourceRefPropertyEditor::OnRefChanged(sh::u32 index)
{
	auto value = m_objectProperty->GetValue(m_object);
	const auto& resourceRef = value.GetResourceRef();
	auto resourceGroup = sh::ResourceManager::GetInstance()->GetResourceGroup(resourceRef.type);

	if (index >= resourceGroup.size())
		return;
}