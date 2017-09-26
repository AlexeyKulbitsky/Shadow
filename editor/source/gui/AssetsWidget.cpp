#include "AssetsWidget.h"

std::weak_ptr<MaterialEditor> s_materialEditor;

FolderTreeItem::FolderTreeItem(TreeItem* parent, sh::io::FileSystemComponent* fsItem)
	: TreeItem(fsItem->name, parent)
{
	m_item = fsItem;
}

FolderTreeItem::~FolderTreeItem()
{
}

void FolderTreeItem::OnContextMenu(sh::s32 x, sh::s32 y)
{
	sh::gui::MenuPtr menu(new sh::gui::Menu());
	menu->AddItem("Add material");
	menu->SetPosition(x, y);
	menu->SetFocus(true);
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
		}
		
		sh::SPtr<MaterialTreeItem> item(new MaterialTreeItem(this, fsItem.get()));
		m_treeWidget->AddItem(item);

		// Create material and add to Resource manager
		sh::video::MaterialPtr material(new sh::video::Material());
		material->SetRenderTechnique("const_color.rt");
		material->SetFileName(materialName);
		material->SetFileInfo(fsItem);
		sh::Device::GetInstance()->GetResourceManager()->AddMaterial(material);

		// Save new material to disk
		material->Save();

		m_treeWidget->UpdateLayout();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////

FileTreeItem::FileTreeItem(TreeItem* parent, sh::io::FileSystemComponent* fsItem)
	: TreeItem(fsItem->name, parent)
{
	m_item = fsItem;
}

FileTreeItem::~FileTreeItem()
{
}

////////////////////////////////////////////////////////////////////////////////////////////

MaterialTreeItem::MaterialTreeItem(TreeItem* parent, sh::io::FileSystemComponent* fsItem)
	: TreeItem(fsItem->name, parent)
{
	m_item = fsItem;
}

MaterialTreeItem::~MaterialTreeItem()
{
}

void MaterialTreeItem::OnToggled(bool toggled)
{
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

////////////////////////////////////////////////////////////////////////////////////////////

AssetsWidget::AssetsWidget()
	:sh::gui::Window(sh::math::Recti(0, 100, 250, 500))
{
	SetText("Assets");
	SetMovable(false);
	sh::gui::VerticalLayoutPtr windowLayout(new sh::gui::VerticalLayout());
	windowLayout->SetMargins(2, 2, 2, 2);

	auto guiMgr = sh::gui::GuiManager::GetInstance();

	SetLayout(windowLayout);

	//////////////////////////////////////////////////

	m_materialEditor.reset(new MaterialEditor());
	guiMgr->AddChild(m_materialEditor);
	m_materialEditor->SetVisible(false);
	s_materialEditor = m_materialEditor;

	struct Local
	{
		static void Parse(const sh::SPtr<sh::io::FileSystemComponent>& item, 
						  const sh::SPtr<TreeWidget>& treeWidget,
						  sh::SPtr<TreeItem>& treeItem)
		{
			if (item->GetType() == sh::io::FileSystemComponent::Type::Folder)
			{
				auto folder = std::static_pointer_cast<sh::io::FolderInfo>(item);
				for (const auto& child : folder->children)
				{
					if (child->GetType() == sh::io::FileSystemComponent::Type::Folder)
					{
						sh::SPtr<TreeItem> childItem(new FolderTreeItem(treeItem.get(), child.get()));
						treeWidget->AddItem(childItem);
						Parse(child, treeWidget, childItem);
					}
					else
					{
						sh::SPtr<TreeItem> childItem;
						size_t pos = child->name.find_last_of('.');
						auto extension = child->name.substr(pos + 1);
						if (extension == "mat")
						{
							sh::SPtr<MaterialTreeItem> item(new MaterialTreeItem(treeItem.get(), child.get()));
							childItem = item;
						}
						else
						{
							childItem.reset(new FileTreeItem(treeItem.get(), child.get()));
						}
						treeWidget->AddItem(childItem);
					}
					
				}
			}
		}
	};

	

	auto root = sh::io::FileSystem::GetInstance()->GetRoot();
	sh::SPtr<AssetsTreeWidget> treeWidget(new AssetsTreeWidget());
	treeWidget->materialChanged.Connect(std::bind(&AssetsWidget::OnMaterialTreeItemChanged, this,
		std::placeholders::_1));

	sh::SPtr<TreeItem> rootTreeItem(new FolderTreeItem(nullptr, root.get()));
	treeWidget->AddItem(rootTreeItem);
	Local::Parse(root, treeWidget, rootTreeItem);

	windowLayout->AddWidget(treeWidget);
}

AssetsWidget::~AssetsWidget()
{
}

void AssetsWidget::OnMaterialTreeItemChanged(const sh::video::MaterialPtr& material)
{
	if (material)
	{
		m_materialEditor->SetMaterial(material);
		m_materialEditor->SetVisible(true);
	}
	else
	{
		m_materialEditor->SetMaterial(nullptr);
		m_materialEditor->SetVisible(false);
	}
}

