#include "AssetsWidget.h"

#include "FolderTreeItem.h"
#include "MaterialTreeItem.h"
#include "TextureTreeItem.h"
#include "ModelTreeItem.h"


std::weak_ptr<MaterialEditor> s_materialEditor;


////////////////////////////////////////////////////////////////////////////////////////////

FileTreeItem::FileTreeItem(TreeItem* parent, sh::io::FileSystemComponent* fsItem)
	: TreeItem(fsItem->name, parent)
{
	m_item = fsItem;
	m_offset += 20U;
	m_layout->SetMargins(0, 0, 0, m_offset);
}

FileTreeItem::~FileTreeItem()
{
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

	m_textureWidget.reset(new TextureWidget());
	guiMgr->AddChild(m_textureWidget);
	m_textureWidget->SetVisible(false);

	struct Local
	{
		static void Parse(const sh::SPtr<sh::io::FileSystemComponent>& item, 
						  const sh::SPtr<sh::gui::TreeWidget>& treeWidget,
						  sh::SPtr<sh::gui::TreeItem>& treeItem)
		{
			if (item->GetType() == sh::io::FileSystemComponent::Type::Folder)
			{
				auto folder = std::static_pointer_cast<sh::io::FolderInfo>(item);
				for (const auto& child : folder->children)
				{
					if (child->GetType() == sh::io::FileSystemComponent::Type::Folder)
					{
						sh::SPtr<sh::gui::TreeItem> childItem(new FolderTreeItem(treeItem.get(), child.get()));
						treeWidget->AddItem(childItem);
						Parse(child, treeWidget, childItem);
					}
					else
					{
						sh::SPtr<sh::gui::TreeItem> childItem;
						size_t pos = child->name.find_last_of('.');
						auto extension = child->name.substr(pos + 1);

						const auto& imageExtensions = sh::video::TextureLoader::GetInstance()->GetAvalilableExtensions();
						const auto& modelExtensions = sh::scene::ModelLoader::GetInstance()->GetAvalilableExtensions();
						if (extension == "mat")
						{
							sh::SPtr<MaterialTreeItem> item(new MaterialTreeItem(treeItem.get(), child.get()));
							childItem = item;
						}
						else
						{
							auto textIt = std::find(imageExtensions.begin(), imageExtensions.end(), extension);
							if (textIt != imageExtensions.end())
							{
								sh::SPtr<TextureTreeItem> item(new TextureTreeItem(treeItem.get(), child.get()));
								childItem = item;
							}
							else
							{
								auto modelIt = std::find(modelExtensions.begin(), modelExtensions.end(), extension);
								if (modelIt != modelExtensions.end())
								{
									sh::SPtr<ModelTreeItem> item(new ModelTreeItem(treeItem.get(), child.get()));
									childItem = item;
								}
								else
								{
									childItem.reset(new FileTreeItem(treeItem.get(), child.get()));
								}
							}
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

	treeWidget->textureChanged.Connect(std::bind(&AssetsWidget::OnTextureTreeItemChanged, this,
		std::placeholders::_1));

	sh::SPtr<sh::gui::TreeItem> rootTreeItem(new FolderTreeItem(nullptr, root.get()));
	treeWidget->AddItem(rootTreeItem);
	Local::Parse(root, treeWidget, rootTreeItem);

	windowLayout->AddWidget(treeWidget);
}

AssetsWidget::~AssetsWidget()
{
}

void AssetsWidget::UpdateLayout()
{
	Window::UpdateLayout();
}

void AssetsWidget::OnMaterialTreeItemChanged(const sh::video::MaterialPtr& material)
{
	if (material)
	{
		m_materialEditor->SetMaterial(material);
		m_materialEditor->SetVisible(true);
		auto rect = m_rect;
		rect.upperLeftCorner.x += m_rect.GetWidth();
		rect.lowerRightCorner.x += m_rect.GetWidth();
		m_materialEditor->SetRect(rect);
	}
	else
	{
		m_materialEditor->SetMaterial(nullptr);
		m_materialEditor->SetVisible(false);
	}
}

void AssetsWidget::OnTextureTreeItemChanged(const sh::video::TexturePtr& texture)
{
	if (texture)
	{
		m_textureWidget->SetTexture(texture);
		m_textureWidget->SetVisible(true);
		auto rect = m_rect;
		rect.upperLeftCorner.x += m_rect.GetWidth();
		rect.lowerRightCorner.x += m_rect.GetWidth();
		m_textureWidget->SetRect(rect);
	}
	else
	{
		m_textureWidget->SetTexture(nullptr);
		m_textureWidget->SetVisible(false);
	}
}

