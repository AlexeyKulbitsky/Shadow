#include "InspectorWidget.h"

InspectorWidget::InspectorWidget()
	: sh::gui::Window(sh::math::Recti(100, 100, 350, 500))
{
	const auto& viewport = sh::Device::GetInstance()->GetDriver()->GetViewPort();
	SetText("Inspector");
	SetMovable(false);
	sh::gui::VerticalLayoutPtr windowLayout(new sh::gui::VerticalLayout());
	m_transformWidget.reset(new TransformWidget());
	m_materialWidget.reset(new MaterialWidget());
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
		m_transformWidget->SetTransformComponent(nullptr);
		m_materialWidget->SetRenderComponent(nullptr);
		m_layout->Clear();
		return;
	}
	// Transform component editor
	sh::Component* component = entity->GetComponent(sh::Component::Type::Transform);
	if (component)
	{
		auto transfromComponent = static_cast<sh::TransformComponent*>(component);
		m_transformWidget->SetTransformComponent(transfromComponent);
		sh::u32 count = m_layout->GetItemsCount();
		bool found = false;
		for (sh::u32 i = 0U; i < count; ++i)
		{
			if (m_transformWidget->GetWidget() == m_layout->GetItem(i)->GetWidget())
			{
				found = true;
				break;
			}
		}
		if (!found)
			m_layout->AddWidget(m_transformWidget->GetWidget());
	}
	// Render component editor
	component = entity->GetComponent(sh::Component::Type::Render);
	if (component)
	{
		auto renderComponent = static_cast<sh::RenderComponent*>(component);
		m_materialWidget->SetRenderComponent(renderComponent);
		sh::u32 count = m_layout->GetItemsCount();
		bool found = false;
		for (sh::u32 i = 0U; i < count; ++i)
		{
			if (m_materialWidget->GetWidget() == m_layout->GetItem(i)->GetWidget())
			{
				found = true;
				break;
			}
		}
		if (!found)
			m_layout->AddWidget(m_materialWidget->GetWidget());
	}
}
