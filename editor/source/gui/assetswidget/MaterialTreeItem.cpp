#include "MaterialTreeItem.h"
#include "AssetsWidget.h"



MaterialTreeItem::MaterialTreeItem(TreeItem* parent, sh::io::FileSystemComponent* fsItem)
	: TreeItem(fsItem->name, parent)
{
	m_item = fsItem;

	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("material");
	auto clonedIcon = icon->Clone();
	clonedIcon->SetMaximumWidth(15);
	m_layout->InsertWidget(0U, clonedIcon);
}

MaterialTreeItem::~MaterialTreeItem()
{
}

void MaterialTreeItem::OnPressed()
{
	sh::gui::TreeItem::OnPressed();

	auto assetsTreeWidget = static_cast<AssetsTreeWidget*>(m_treeWidget);
	auto selectedItem = m_treeWidget->GetSelectedItem();
	if (selectedItem)
	{
		auto material = sh::Device::GetInstance()->GetResourceManager()->GetMaterial(m_item->name);
		assetsTreeWidget->materialChanged(material);
	}
	else
	{
		assetsTreeWidget->materialChanged(nullptr);
	}
}


void MaterialTreeItem::OnContextMenu(sh::s32 x, sh::s32 y)
{
	sh::gui::MenuPtr menu(new sh::gui::Menu());
	menu->AddItem("Rename");
	menu->SetPosition(x, y);
	menu->itemSelected.Connect(std::bind(&MaterialTreeItem::OnMenuItemSelected, this, std::placeholders::_1));
	sh::gui::GuiManager::GetInstance()->SetFocusWidget(menu);
}

void MaterialTreeItem::OnEdit(sh::s32 x, sh::s32 y)
{
	//SH_ASSERT(0);
}

void MaterialTreeItem::OnMenuItemSelected(const sh::String& itemName)
{
	auto label = std::static_pointer_cast<sh::gui::Label>(m_layout->GetItem(1U)->GetWidget());

	sh::gui::LineEditPtr lineEdit(new sh::gui::LineEdit());

	const auto& originalFileName = label->GetText();
	size_t pos = originalFileName.find_last_of('.');
	const sh::String name = originalFileName.substr(0U, pos);

	lineEdit->SetText(name);
	lineEdit->SetRect(label->GetRect());
	lineEdit->SetState(sh::gui::LineEdit::State::Edit);
	lineEdit->OnEditingFinished.Connect([this](const sh::String& text)
	{
		sh::gui::GuiManager::GetInstance()->SetFocusWidget(nullptr);

		// Check if new name is available
		sh::io::FileSystem* fs = sh::Device::GetInstance()->GetFileSystem();
		const auto& materialFileNames = fs->GetMaterialFileInfos();
		bool fileNameAlreadyExists = false;
		const sh::String fileName = text + ".mat";
		for (const auto& materialFilename : materialFileNames)
		{
			if (fileName == materialFilename.lock()->name)
			{
				fileNameAlreadyExists = true;
				break;
			}
		}
		if (fileNameAlreadyExists)
			return;

		// Rename material
		const auto& material = sh::Device::GetInstance()->GetResourceManager()->GetMaterial(m_item->name);

		m_item->Rename(fileName);
		auto label = std::static_pointer_cast<sh::gui::Label>(m_layout->GetItem(1U)->GetWidget());
		label->SetText(fileName);

		// Save renamed material
		material->Save();
	});
	//lineEdit->SetFocus(true);
	//sh::gui::GuiManager::GetInstance()->SetFocusWidget(lineEdit);
}