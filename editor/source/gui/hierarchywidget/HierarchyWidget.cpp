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

void HierarchyTreeWidget::SetSelectedItem(sh::gui::TreeItem* item)
{
	sh::gui::TreeWidget::SetSelectedItem(item);
	if (!item)
		return;

	auto hierarchyItem = static_cast<HeirarchyTreeItem*>(item);
	auto entity = hierarchyItem->GetEntity();

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
}
