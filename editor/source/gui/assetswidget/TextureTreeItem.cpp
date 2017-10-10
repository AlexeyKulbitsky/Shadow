#include "TextureTreeItem.h"
#include "AssetsWidget.h"

TextureTreeItem::TextureTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem)
	: TreeItem(fsItem->name, parent)
{
	m_item = fsItem;

	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("image");
	auto clonedIcon = icon->Clone();
	clonedIcon->SetMaximumWidth(15);
	m_layout->InsertWidget(0U, clonedIcon);
}

TextureTreeItem::~TextureTreeItem()
{

}

void TextureTreeItem::OnPressed()
{
	sh::gui::TreeItem::OnPressed();

	auto assetsTreeWidget = static_cast<AssetsTreeWidget*>(m_treeWidget);
	auto selectedItem = m_treeWidget->GetSelectedItem();
	if (selectedItem)
	{
		auto texture = sh::Device::GetInstance()->GetResourceManager()->GetTexture(m_item->name);
		assetsTreeWidget->textureChanged(texture);
	}
	else
	{
		assetsTreeWidget->textureChanged(nullptr);
	}
}
