#include "AssetsWidget.h"

AssetsWidget::AssetsWidget()
{
	m_window.reset(new sh::gui::Window(sh::math::Recti(0, 300, 250, 500)));
	const auto& viewport = sh::Device::GetInstance()->GetDriver()->GetViewPort();
	m_window->SetText("Assets");

	sh::gui::VerticalLayoutPtr windowLayout(new sh::gui::VerticalLayout());
	windowLayout->SetMargins(2, 2, 2, 2);

	auto guiMgr = sh::gui::GuiManager::GetInstance();
	auto buttonPrefab = guiMgr->GetStyle()->GetButton("TreeButton");
	buttonPrefab->SetMaximumSize(sh::math::Vector2u(10));
	buttonPrefab->SetToggleable(true);
	windowLayout->AddWidget(buttonPrefab->Clone());
	windowLayout->AddWidget(buttonPrefab->Clone());
	windowLayout->AddWidget(buttonPrefab->Clone());
	windowLayout->AddWidget(buttonPrefab->Clone());

	m_window->SetLayout(windowLayout);

	sh::gui::GuiManager::GetInstance()->AddChild(m_window);
}

AssetsWidget::~AssetsWidget()
{
}