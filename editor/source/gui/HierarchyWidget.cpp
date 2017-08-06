#include "HierarchyWidget.h"

HierarchyWidget::HierarchyWidget()
{
	m_window.reset(new sh::gui::Window(sh::math::Rectu(100, 100, 250, 500)));
	const auto& viewport = sh::Device::GetInstance()->GetDriver()->GetViewPort();
	UpdateGeometry(viewport.z, viewport.w);
	m_window->SetText("Hierarchy");
	m_layout.reset(new sh::gui::VerticalLayout());
	m_window->SetLayout(m_layout);
	m_window->SetMovable(false);

	sh::gui::GuiManager::GetInstance()->AddChild(m_window);

	sh::Device::GetInstance()->windowResizeEvent.Connect(std::bind(&HierarchyWidget::OnWindowResized, this,
		std::placeholders::_1, std::placeholders::_2));
}

HierarchyWidget::~HierarchyWidget()
{

}

void HierarchyWidget::AddEntity(sh::Entity* entity)
{
	sh::gui::ButtonPtr button(new sh::gui::Button(entity->GetName()));
	button->SetToggleable(true);
	button->SetMaximumHeight(25U);
	button->OnToggle.Connect(std::bind(&HierarchyWidget::OnItemToggled, this,
		std::placeholders::_1, std::placeholders::_2));
	m_window->GetLayout()->AddWidget(button);
}

void HierarchyWidget::SetSelectedEntity(sh::Entity* entity)
{
	if (entity == nullptr)
	{
		if (m_currentSelectedItem)
		{
			m_currentSelectedItem->SetToggled(false);
		}
		m_currentSelectedItem == nullptr;
		return;
	}
	auto sceneMgr = sh::Device::GetInstance()->GetSceneManager();
	sh::u32 entitiesCount = sceneMgr->GetEntitiesCount();
	for (sh::u32 i = 0U; i < entitiesCount; ++i)
	{
		auto ent = sceneMgr->GetEntity(i);
		if (ent == entity)
		{
			const auto& button = std::static_pointer_cast<sh::gui::Button>(m_layout->GetWidget(i));
			if (m_currentSelectedItem && m_currentSelectedItem != button)
			{
				m_currentSelectedItem->SetToggled(false);
			}
			button->SetToggled(true);
			m_currentSelectedItem = button;
			break;
		}
	}
}

void HierarchyWidget::OnWindowResized(sh::s32 width, sh::s32 height)
{
	UpdateGeometry(static_cast<sh::u32>(width), static_cast<sh::u32>(height));
}

void HierarchyWidget::OnItemToggled(bool toggled, const sh::gui::ButtonPtr& sender)
{
	if (!toggled)
	{
		if (m_currentSelectedItem)
		{
			m_currentSelectedItem = nullptr;
			OnEntitySelected(nullptr);
		}
		return;
	}

	

	sh::u32 itemsCount = m_layout->GetItemsCount();
	for (sh::u32 i = 0U; i < itemsCount; ++i)
	{
		const auto& button = std::static_pointer_cast<sh::gui::Button>(m_layout->GetWidget(i));
		if (button != sender && button->IsToggled())
		{
			button->SetToggled(false);
		}
		if (button == sender && button != m_currentSelectedItem)
		{
			SelectEntity(i);
		}
	}

	m_currentSelectedItem = sender;
}

void HierarchyWidget::UpdateGeometry(sh::u32 screenWidth, sh::u32 screenHeight)
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
	m_window->SetPosition(0U, verticalOffset);
	m_window->SetHeight(screenHeight - verticalOffset);
}

void HierarchyWidget::SelectEntity(sh::u32 index)
{
	auto entity = sh::Device::GetInstance()->GetSceneManager()->GetEntity(index);
	OnEntitySelected(entity);
}