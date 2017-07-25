#include "InspectorWidget.h"

InspectorWidget::InspectorWidget()
{
	sh::gui::WindowPtr window(new sh::gui::Window(sh::math::Rectu(100, 100, 500, 200)));
	window->SetText("Inspector");
	sh::gui::VerticalLayoutPtr windowLayout(new sh::gui::VerticalLayout());
	m_transformWidget.reset(new TransformWidget());
	windowLayout->AddWidget(m_transformWidget->GetWidget());
	window->SetLayout(windowLayout);

	sh::gui::GuiManager::GetInstance()->AddChild(window);
}

InspectorWidget::~InspectorWidget()
{

}

void InspectorWidget::SetEntity(sh::Entity* entity)
{
	m_entity = entity;

	if (!entity)
	{
		m_transformWidget->SetTransformComponent(nullptr);
		return;
	}

	sh::Component* component = entity->GetComponent(sh::Component::Type::Transform);
	if (component)
	{
		auto transfromComponent = static_cast<sh::TransformComponent*>(component);
		m_transformWidget->SetTransformComponent(transfromComponent);
	}
}