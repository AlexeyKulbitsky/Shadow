#include "InspectorWidget.h"

InspectorWidget::InspectorWidget()
{
	m_window.reset(new sh::gui::Window(sh::math::Recti(100, 100, 350, 500)));
	const auto& viewport = sh::Device::GetInstance()->GetDriver()->GetViewPort();
	UpdateGeometry(viewport.z, viewport.w);
	m_window->SetText("Inspector");
	m_window->SetMovable(false);
	sh::gui::VerticalLayoutPtr windowLayout(new sh::gui::VerticalLayout());
	m_transformWidget.reset(new TransformWidget());
	//windowLayout->AddWidget(m_transformWidget->GetWidget());
	m_window->SetLayout(windowLayout);

	for (sh::u32 i = 0; i < 10; ++i)
	{
		std::stringstream s;
		s << "Button " << i;

		sh::gui::ButtonPtr button(new sh::gui::Button(s.str()));
		m_window->GetLayout()->AddWidget(button);
	}

	sh::gui::GuiManager::GetInstance()->AddChild(m_window);

	sh::Device::GetInstance()->windowResizeEvent.Connect(std::bind(&InspectorWidget::OnWindowResized, this,
		std::placeholders::_1, std::placeholders::_2));
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
		m_window->GetLayout()->Clear();
		return;
	}

	sh::Component* component = entity->GetComponent(sh::Component::Type::Transform);
	if (component)
	{
		auto transfromComponent = static_cast<sh::TransformComponent*>(component);
		m_transformWidget->SetTransformComponent(transfromComponent);
		sh::u32 count = m_window->GetLayout()->GetItemsCount();
		bool found = false;
		for (sh::u32 i = 0U; i < count; ++i)
		{
			if (m_transformWidget->GetWidget() == m_window->GetLayout()->GetItem(i)->GetWidget())
			{
				found = true;
				break;
			}
		}
		if (!found)
			m_window->GetLayout()->AddWidget(m_transformWidget->GetWidget());
	}
}

void InspectorWidget::OnWindowResized(sh::s32 width, sh::s32 height)
{
	UpdateGeometry(static_cast<sh::u32>(width), static_cast<sh::u32>(height));
}

void InspectorWidget::UpdateGeometry(sh::u32 screenWidth, sh::u32 screenHeight)
{
	sh::u32 windowWidth = m_window->GetRect().GetWidth();
	sh::u32 windowHeight = m_window->GetRect().GetHeight();

	sh::u32 verticalOffset = 0U;
	auto menuBar = sh::gui::GuiManager::GetInstance()->GetMenuBar();
	if (menuBar)
		verticalOffset += menuBar->GetRect().GetHeight();

	auto toolBar = sh::gui::GuiManager::GetInstance()->GetToolBar();
	if (toolBar)
		verticalOffset += toolBar->GetRect().GetHeight();
	m_window->SetPosition(screenWidth - windowWidth, verticalOffset);
	m_window->SetHeight(screenHeight - verticalOffset);
}