#include "HierarchyWidget.h"


//////////////////////////////////////////////////////////////////////////////////////

HeirarchyTreeItem::HeirarchyTreeItem(TreeItem* parent, sh::Entity* entity)
	:TreeItem(entity->GetName(), parent)
{
	m_entity = entity;
}

//////////////////////////////////////////////////////////////////////////////////////

void HeirarchyTreeItem::OnDoubleClick(sh::s32 x, sh::s32 y)
{
	if (m_treeWidget->GetSelectedItem() == this)
	{

	}
	else
	{

	}
}

//////////////////////////////////////////////////////////////////////////////////////

HierarchyTreeWidget::HierarchyTreeWidget()
{
}

//////////////////////////////////////////////////////////////////////////////////////

void HierarchyTreeWidget::SetSelectedItem(sh::gui::TreeItem* item)
{
	sh::gui::TreeWidget::SetSelectedItem(item);
	if (item)
	{
		auto hierarchyItem = static_cast<HeirarchyTreeItem*>(item);
		auto entity = hierarchyItem->GetEntity();
		m_hierarchyWidget->OnEntitySelected(entity);
	}
	else
	{
		m_hierarchyWidget->OnEntitySelected(nullptr);
	}
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

	sh::gui::SliderWidgetPtr slider(new sh::gui::SliderWidget());
	m_layout->AddWidget(slider);

	m_treeWidget.reset(new HierarchyTreeWidget());
	m_treeWidget->SetHierarchyWidget(this);
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
	if (entity)
	{
		const auto itemsCount = m_treeWidget->GetLayout()->GetItemsCount();
		for (size_t i = 0; i < itemsCount; ++i)
		{
			auto hierarchyItem = static_cast<HeirarchyTreeItem*>(m_treeWidget->GetLayout()->GetWidget(i).get());
			auto itemEntity = hierarchyItem->GetEntity();
			if (itemEntity == entity)
			{
				m_treeWidget->SetSelectedItem(hierarchyItem);
				return;
			}
		}
		m_treeWidget->SetSelectedItem(nullptr);
	}
	else
	{
		m_treeWidget->SetSelectedItem(nullptr);
	}
}
