#include "MaterialTreeItem.h"
#include "AssetsWidget.h"



MaterialTreeItem::MaterialTreeItem(TreeItem* parent, sh::io::FileSystemComponent* fsItem)
	: TreeItem(fsItem->name, parent)
{
	m_item = fsItem;
	m_offset += 20U;
	m_layout->SetMargins(0, 0, 0, m_offset);

	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("material");
	auto clonedIcon = icon->Clone();
	clonedIcon->SetMaximumWidth(15);
	m_layout->InsertWidget(0U, clonedIcon);
}

MaterialTreeItem::~MaterialTreeItem()
{
}

void MaterialTreeItem::OnToggled(bool toggled)
{
	sh::gui::TreeItem::OnToggled(toggled);

	auto assetsTreeWidget = static_cast<AssetsTreeWidget*>(m_treeWidget);
	if (toggled)
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
	menu->SetFocus(true);
	menu->itemSelected.Connect(std::bind(&MaterialTreeItem::OnMenuItemSelected, this, std::placeholders::_1));
	sh::gui::GuiManager::GetInstance()->SetFocusWidget(menu);
}

void MaterialTreeItem::OnEdit(sh::s32 x, sh::s32 y)
{
	//SH_ASSERT(0);
}

void MaterialTreeItem::OnMenuItemSelected(const sh::String& itemName)
{
	auto button = std::static_pointer_cast<sh::gui::Button>(m_layout->GetItem(0U)->GetWidget());

	sh::gui::LineEditPtr lineEdit(new sh::gui::LineEdit());

	const auto& originalFileName = button->GetText();
	size_t pos = originalFileName.find_last_of('.');
	const sh::String name = originalFileName.substr(0U, pos);

	lineEdit->SetText(name);
	lineEdit->SetRect(button->GetRect());
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
		// 		m_item->name = fileName;
		// 		size_t separatorPos = m_item->absolutePath.find_last_of('/');
		// 		m_item->absolutePath = m_item->absolutePath.substr(0U, separatorPos) + "/" + fileName;
		// 		auto button = std::static_pointer_cast<sh::gui::Button>(m_layout->GetItem(0U)->GetWidget());
		// 		button->SetText(fileName);

		m_item->Rename(fileName);
		auto button = std::static_pointer_cast<sh::gui::Button>(m_layout->GetItem(0U)->GetWidget());
		button->SetText(fileName);

		// Save renamed material
		material->Save();

		//sh::gui::GuiManager::GetInstance()->SetFocusWidget(nullptr);
	});
	lineEdit->SetFocus(true);
	sh::gui::GuiManager::GetInstance()->SetFocusWidget(lineEdit);
}