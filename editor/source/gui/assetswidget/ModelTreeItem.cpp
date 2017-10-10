#include "ModelTreeItem.h"

#include "AssetsWidget.h"

ModelTreeItem::ModelTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem)
	: TreeItem(fsItem->name, parent)
{
	m_item = fsItem;

	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("model");
	auto clonedIcon = icon->Clone();
	clonedIcon->SetMaximumWidth(15);
	m_layout->InsertWidget(0U, clonedIcon);
}

ModelTreeItem::~ModelTreeItem()
{

}

void ModelTreeItem::OnPressed()
{
	sh::gui::TreeItem::OnPressed();
}