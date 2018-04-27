#include "InspectorWidget.h"
#include "ComponentEditor.h"

InspectorWidget::InspectorWidget()
	: sh::gui::Window(sh::math::Rect(100, 100, 350, 500))
{
	const auto& viewport = sh::Device::GetInstance()->GetDriver()->GetViewPort();
	SetText("Inspector");
	SetMovable(false);
	sh::gui::VerticalLayoutPtr windowLayout(new sh::gui::VerticalLayout());
	m_transformComponentWidget.reset(new TransformComponentWidget());
	m_transformComponentWidget->SetParent(this);
	m_renderComponentWidget.reset(new RenderComponentWidget());
	m_lightComponentWidget.reset(new LightComponentWidget());
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
    
    auto transfromComponent = entity->GetComponent<sh::TransformComponent>();
    if (transfromComponent)
    {
        m_transformComponentWidget->SetTransformComponent(transfromComponent);
    }

#if 0
	// Clear layout
	m_transformComponentWidget->SetTransformComponent(nullptr);
	m_renderComponentWidget->SetRenderComponent(nullptr);
	m_lightComponentWidget->SetLightComponent(nullptr);
	m_layout->Clear();

	if (!entity)
	{
		return;
	}

	// Transform component editor
	auto transfromComponent = entity->GetComponent<sh::TransformComponent>();
	if (transfromComponent)
	{
		m_transformComponentWidget->SetTransformComponent(transfromComponent);
		m_layout->AddWidget(m_transformComponentWidget);
	}
	// Render component editor
	auto renderComponent = entity->GetComponent<sh::RenderComponent>();
	if (renderComponent)
	{
		m_renderComponentWidget->SetRenderComponent(renderComponent);
		m_layout->AddWidget(m_renderComponentWidget);
	}
	// Light component editor
	auto lightComponent = entity->GetComponent<sh::LightComponent>();
	if (lightComponent)
	{
		m_lightComponentWidget->SetLightComponent(lightComponent);
		m_layout->AddWidget(m_lightComponentWidget);
	}
#endif
}
