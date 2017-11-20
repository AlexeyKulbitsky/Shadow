#include "FolderTreeItem.h"
#include "MaterialTreeItem.h"
#include "AssetsWidget.h"

FolderTreeItem::FolderTreeItem(TreeItem* parent, sh::io::FileSystemComponent* fsItem)
	: TreeItem(fsItem->name, parent)
{
	m_item = fsItem;

	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("folder");
	auto clonedIcon = icon->Clone();
	clonedIcon->SetMaximumWidth(15);
	m_layout->InsertWidget(0U, clonedIcon);
}

FolderTreeItem::~FolderTreeItem()
{
}

void FolderTreeItem::OnExpanded(bool expanded)
{
	SetExpanded(!expanded);
	m_treeWidget->UpdateLayout();
}

void FolderTreeItem::OnContextMenu(sh::s32 x, sh::s32 y)
{
	sh::gui::MenuPtr menu(new sh::gui::Menu());
	menu->AddItem("Add material");
	menu->AddItem("Add folder");
	menu->SetPosition(x, y);
	//menu->SetFocus(true);
	menu->itemSelected.Connect(std::bind(&FolderTreeItem::OnMenuItemSelected, this, std::placeholders::_1));
	sh::gui::GuiManager::GetInstance()->SetFocusWidget(menu);
}

void FolderTreeItem::OnMenuItemSelected(const sh::String& itemName)
{
	if (itemName == "Add material")
	{
		static size_t matCounter = 0U;
		sh::io::FileSystem* fs = sh::Device::GetInstance()->GetFileSystem();
		std::stringstream ss;
		const auto& materialFileNames = fs->GetMaterialFileInfos();
		bool isMaterialnameFree = false;

		// Find free material name
		while (!isMaterialnameFree)
		{
			isMaterialnameFree = true;
			ss.str(std::string());
			ss << "New_material_" << matCounter++ << ".mat";
			const std::string s(ss.str());
			for (const auto& materialFilename : materialFileNames)
			{
				if (s == materialFilename.lock()->name)
				{
					isMaterialnameFree = false;
					break;
				}
			}
		}

		// Create new fileentry description
		sh::String materialName(ss.str());
		sh::SPtr<sh::io::FileInfo> fsItem(new sh::io::FileInfo(materialName, m_item->absolutePath + materialName));
		if (m_item->GetType() == sh::io::FileSystemComponent::Type::Folder)
		{
			sh::io::FolderInfo* folderInfo = static_cast<sh::io::FolderInfo*>(m_item);
			folderInfo->children.push_back(fsItem);
			// Update resource groups in file system
			fs->UpdateResourceGroups();
			sh::Device::GetInstance()->GetResourceManager()->UpdateResourceGroups();
		}

		sh::SPtr<MaterialTreeItem> item(new MaterialTreeItem(this, fsItem.get()));
		m_treeWidget->AddItem(item);

		// Create material and add to Resource manager
		sh::video::MaterialPtr material(new sh::video::Material());
		material->SetRenderTechnique("Default.rt");
		//material->SetFileName(materialName);
		material->SetFileInfo(fsItem);
		sh::Device::GetInstance()->GetResourceManager()->AddMaterial(material);

		// Save new material to disk
		material->Save();

		m_treeWidget->UpdateLayout();
	}
	else if (itemName == "Add folder")
	{
		sh::io::FileSystem* fs = sh::Device::GetInstance()->GetFileSystem();
		std::stringstream ss;

		bool isFoldeNameFree = false;
		size_t folderCounter = 0U;
		// Find free material name
		while (!isFoldeNameFree)
		{
			isFoldeNameFree = true;
			ss.str(std::string());
			ss << "New_folder_" << folderCounter++;
			const std::string s(ss.str());
			auto folder = static_cast<sh::io::FolderInfo*>(m_item);
			for (auto childItem : folder->children)
			{
				if (childItem->GetType() == sh::io::FileSystemComponent::Type::Folder &&
					s == childItem->name)
				{
					isFoldeNameFree = false;
					break;
				}
			}
		}

		// Create new fileentry description
		sh::String folderName(ss.str());
		fs->CreateFolder(m_item->absolutePath + folderName);
		sh::SPtr<sh::io::FolderInfo> fsItem(new sh::io::FolderInfo(folderName, m_item->absolutePath + folderName + "/"));
		if (m_item->GetType() == sh::io::FileSystemComponent::Type::Folder)
		{
			sh::io::FolderInfo* folderInfo = static_cast<sh::io::FolderInfo*>(m_item);
			folderInfo->children.push_back(fsItem);
		}

		sh::SPtr<FolderTreeItem> item(new FolderTreeItem(this, fsItem.get()));
		m_treeWidget->AddItem(item);
		m_treeWidget->UpdateLayout();
	}
}