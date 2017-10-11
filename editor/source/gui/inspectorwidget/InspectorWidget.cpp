#include "InspectorWidget.h"

InspectorWidget::InspectorWidget()
	: sh::gui::Window(sh::math::Recti(100, 100, 350, 500))
{
	const auto& viewport = sh::Device::GetInstance()->GetDriver()->GetViewPort();
	SetText("Inspector");
	SetMovable(false);
	sh::gui::VerticalLayoutPtr windowLayout(new sh::gui::VerticalLayout());
	m_transformComponentWidget.reset(new TransformComponentWidget());
	m_transformComponentWidget->SetParent(this);
	m_renderComponentWidget.reset(new RenderComponentWidget());
	SetLayout(windowLayout);
}

InspectorWidget::~InspectorWidget()
{

}

void InspectorWidget::SetEntity(sh::Entity* entity)
{
	m_entity = entity;

	if (!entity)
	{
		m_transformComponentWidget->SetTransformComponent(nullptr);
		m_renderComponentWidget->SetRenderComponent(nullptr);
		m_layout->Clear();
		return;
	}
	// Transform component editor
	auto transfromComponent = entity->GetComponent<sh::TransformComponent>();
	if (transfromComponent)
	{
		m_transformComponentWidget->SetTransformComponent(transfromComponent);
		sh::u32 count = m_layout->GetItemsCount();
		bool found = false;
		for (sh::u32 i = 0U; i < count; ++i)
		{
			if (m_transformComponentWidget == m_layout->GetItem(i)->GetWidget())
			{
				found = true;
				break;
			}
		}
		if (!found)
			m_layout->AddWidget(m_transformComponentWidget);
	}
	// Render component editor
	auto renderComponent = entity->GetComponent<sh::RenderComponent>();
	if (renderComponent)
	{
		m_renderComponentWidget->SetRenderComponent(renderComponent);
		sh::u32 count = m_layout->GetItemsCount();
		bool found = false;
		for (sh::u32 i = 0U; i < count; ++i)
		{
			if (m_renderComponentWidget == m_layout->GetItem(i)->GetWidget())
			{
				found = true;
				break;
			}
		}
		if (!found)
			m_layout->AddWidget(m_renderComponentWidget);
	}
}
