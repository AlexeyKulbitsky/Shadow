#include "InspectorWidget.h"
#include "ComponentEditor.h"

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
