#include "InspectorWidget.h"
#include "ComponentEditor.h"
#include "../propertyeditors/PropertyEditorsFactory.h"

InspectorWidget::InspectorWidget()
	: sh::gui::Window(sh::math::Rect(100, 100, 350, 500))
{
	SetText("Inspector");
	SetMovable(false);
	sh::gui::VerticalLayoutPtr windowLayout(new sh::gui::VerticalLayout());
	SetLayout(windowLayout);
}

InspectorWidget::~InspectorWidget()
{

}

void InspectorWidget::SetEntity(sh::Entity* entity)
{
	m_entity = entity;

	m_layout->Clear();

	if (!m_entity)
		return;

    // Read the properties of the entity itself
    auto properties = sh::ObjectFactory::GetInstance()->GetProperties(m_entity->GetTypeName());
    if (properties)
    {
        for (const auto& property : (*properties))
        {
            auto propertyEditor = PropertyEditorsFactory::GetInstance()->CreatePropertyEditor(m_entity, property.second);
            if (propertyEditor)
                m_layout->AddWidget(propertyEditor);
        }
    }
    
    // Read the components
	const auto& components = m_entity->GetComponents();
	for (auto component : components)
	{
		if (!component)
			continue;

		sh::gui::WidgetPtr componentWidget(new ComponentEditor(component));
		componentWidget->SetParent(this);
		m_layout->AddWidget(componentWidget);
	}
}
