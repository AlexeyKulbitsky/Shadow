#include "ModelTreeItem.h"

#include "AssetsWidget.h"

ModelTreeItem::ModelTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem)
	: TreeItem(fsItem->name, parent)
{
	m_item = fsItem;
	m_offset += 20U;
	m_layout->SetMargins(0, 0, 0, m_offset);

	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("model");
	auto clonedIcon = icon->Clone();
	clonedIcon->SetMaximumWidth(15);
	m_layout->InsertWidget(0U, clonedIcon);
}

ModelTreeItem::~ModelTreeItem()
{

}

void ModelTreeItem::OnToggled(bool toggled)
{
	sh::gui::TreeItem::OnToggled(toggled);

// 	auto assetsTreeWidget = static_cast<AssetsTreeWidget*>(m_treeWidget);
// 	if (toggled)
// 	{
// 		auto texture = sh::Device::GetInstance()->GetResourceManager()->GetTexture(m_item->name);
// 		assetsTreeWidget->textureChanged(texture);
// 	}
// 	else
// 	{
// 		assetsTreeWidget->textureChanged(nullptr);
// 	}
}