#include "HierarchyWidget.h"


//////////////////////////////////////////////////////////////////////////////////////

HeirarchyTreeItem::HeirarchyTreeItem(TreeItem* parent, sh::Entity* entity)
	:TreeItem(entity->GetName(), parent)
{
	m_entity = entity;
}

//////////////////////////////////////////////////////////////////////////////////////

HierarchyTreeWidget::HierarchyTreeWidget()
{
}

//////////////////////////////////////////////////////////////////////////////////////

HierarchyWidget::HierarchyWidget()
	: sh::gui::Window(sh::math::Recti(0, 50, 250, 400))
{
	SetText("Hierarchy");
	SetMovable(false);
	m_layout.reset(new sh::gui::VerticalLayout());
	m_layout->SetMargins(2, 2, 2, 2);
	SetLayout(m_layout);

	m_treeWidget.reset(new HierarchyTreeWidget());
	m_layout->AddWidget(m_treeWidget);
}

HierarchyWidget::~HierarchyWidget()
{

}

void HierarchyWidget::AddEntity(sh::Entity* entity)
{
	sh::SPtr<HeirarchyTreeItem> item(new HeirarchyTreeItem(nullptr, entity));
	m_treeWidget->AddItem(item);
}

void HierarchyWidget::SetSelectedEntity(sh::Entity* entity)
{
// 	if (entity == nullptr)
// 	{
// 		if (m_currentSelectedItem)
// 		{
// 			m_currentSelectedItem->SetToggled(false);
// 		}
// 		m_currentSelectedItem == nullptr;
// 		return;
// 	}
// 	auto sceneMgr = sh::Device::GetInstance()->GetSceneManager();
// 	sh::u32 entitiesCount = sceneMgr->GetEntitiesCount();
// 	for (sh::u32 i = 0U; i < entitiesCount; ++i)
// 	{
// 		auto ent = sceneMgr->GetEntity(i);
// 		if (ent == entity)
// 		{
// 			const auto& button = std::static_pointer_cast<sh::gui::Button>(m_layout->GetWidget(i));
// 			if (m_currentSelectedItem && m_currentSelectedItem != button)
// 			{
// 				m_currentSelectedItem->SetToggled(false);
// 			}
// 			button->SetToggled(true);
// 			m_currentSelectedItem = button;
// 			break;
// 		}
// 	}
}

void HierarchyWidget::OnItemToggled(bool toggled, const sh::gui::ButtonPtr& sender)
{
	if (!toggled)
	{
		if (m_currentSelectedItem == sender)
		{
			//m_currentSelectedItem = nullptr;
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

void HierarchyWidget::SelectEntity(sh::u32 index)
{
	auto entity = sh::Device::GetInstance()->GetSceneManager()->GetEntity(index);
	OnEntitySelected(entity);
}