#include "AssetsWidget.h"

TreeItem::TreeItem(const sh::String& name, TreeItem* parent) 
	: m_name(name)
	, m_parent(parent)
{  
	sh::gui::ButtonPtr button(new sh::gui::Button(name));
	button->SetToggleable(true);

	button->OnToggle.Connect(std::bind(&TreeItem::OnToggled, this, std::placeholders::_1));
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	layout->AddWidget(button);
	SetMinimumHeight(15U);
	SetMaximumHeight(20U);
	SetLayout(layout);

	if (parent)
	{
		sh::s32 offset = parent->GetOffset();
		m_offset = offset + 15U;
		layout->SetMargins(0, 0, 0, m_offset);
	}
}

TreeItem::~TreeItem() 
{ 
}

void TreeItem::AddChild(const sh::SPtr<TreeItem>& child)
{
	m_children.push_back(child);
}

void TreeItem::SetExpanded(bool expanded)
{ 
	m_expanded = expanded; 
}

void TreeItem::OnToggled(bool toggled)
{
	if (m_children.size() == 0U)
	{
		m_treeWidget->itemToggled(m_name, toggled);
	}
	SetExpanded(!toggled);
	m_treeWidget->UpdateLayout();
}

bool TreeItem::ProcessEvent(sh::gui::GUIEvent& ev)
{
	if (ev.type == sh::gui::EventType::PointerDown &&
		ev.mouseButtonCode == sh::MouseCode::ButtonRight)
		return false;

	if (ev.type == sh::gui::EventType::PointerUp &&
		ev.mouseButtonCode == sh::MouseCode::ButtonRight &&
		m_rect.IsPointInside(ev.x, ev.y))
	{
		sh::gui::MenuPtr menu(new sh::gui::Menu());
		menu->AddItem("Item 1");
		menu->AddItem("Item 2");
		menu->AddItem("Item 3");
		menu->AddItem("Item 4");
		menu->AddItem("Item 5");
		menu->SetPosition(ev.x, ev.y);
		sh::gui::GuiManager::GetInstance()->SetFocusWidget(menu);

		return true;
	}
	else
	{
		return Widget::ProcessEvent(ev);
	}
}

//////////////////////////////////////////////////////////////////////////////////

TreeWidget::TreeWidget()
{
	m_layout.reset(new sh::gui::VerticalLayout());
	m_layout->SetSpacing(1);
	SetLayout(m_layout);
}

sh::SPtr<TreeItem> TreeWidget::AddItem(const sh::String& name, TreeItem* parent)
{
	sh::SPtr<TreeItem> childItem(new TreeItem(name, parent));
	childItem->m_treeWidget = this;
	m_layout->AddWidget(childItem);
	return childItem;
}

void TreeWidget::Render(sh::video::Painter* painter)
{
	painter->SetClipRect(sh::math::Rectu(m_rect.upperLeftCorner.x, m_rect.upperLeftCorner.y,
		m_rect.lowerRightCorner.x, m_rect.lowerRightCorner.y));
	
	if (m_layout)
	{
		const sh::u32 itemsCount = m_layout->GetItemsCount();
		for (sh::u32 i = 0U; i < itemsCount; ++i)
		{
			auto treeItem = std::static_pointer_cast<TreeItem>(m_layout->GetWidget(i));
			if (treeItem->IsVisible())
				treeItem->Render(painter);
		}
	}

	painter->SetClipRect(sh::Device::GetInstance()->GetDriver()->GetViewport());
}

void TreeWidget::UpdateLayout()
{
	if (m_layout)
	{
		const sh::u32 itemsCount = m_layout->GetItemsCount();
		sh::u32 height = 0U;
		auto startPos = m_rect.upperLeftCorner;

		for (sh::u32 i = 0U; i < itemsCount; ++i)
		{
			auto treeItem = std::static_pointer_cast<TreeItem>(m_layout->GetWidget(i));

			TreeItem* parent = treeItem->GetParent();
			bool expanded = true;
			while(parent)
			{
				if (!parent->IsExpanded())
				{
					expanded = false;
					break;
				}
				parent = parent->GetParent();
			}

			if (!expanded)
			{
				treeItem->SetVisible(false);
				treeItem->SetMaximumHeight(0U);
				treeItem->SetHeight(0U);
				continue;
			}
			else
			{
				treeItem->SetVisible(true);
				treeItem->SetMaximumHeight(15U);
				treeItem->SetHeight(15U);
			}

			treeItem->SetPosition(startPos.x, startPos.y + height);
			height += m_layout->GetItem(i)->GetHeight() + m_layout->GetSpacing();
		}
		auto r = m_rect;
		r.lowerRightCorner.y = r.upperLeftCorner.y + height;


		sh::s32 delta = m_fullRect.upperLeftCorner.y - r.upperLeftCorner.y;
		r.lowerRightCorner.y += delta;
		r.upperLeftCorner.y += delta;
		if (r.lowerRightCorner.y < m_rect.lowerRightCorner.y)
		{
			sh::s32 delta = m_rect.lowerRightCorner.y - r.lowerRightCorner.y;
			r.lowerRightCorner.y += delta;
			r.upperLeftCorner.y += delta;
		}
		if (r.upperLeftCorner.y > m_rect.upperLeftCorner.y)
		{
			sh::s32 delta = m_rect.upperLeftCorner.y - r.upperLeftCorner.y;
			r.lowerRightCorner.y += delta;
			r.upperLeftCorner.y += delta;
		}

		m_layout->Resize(r);
		m_fullRect = r;

		m_scrollSpeed = height / 25;
	}
}


//////////////////////////////////////////////////////////////////////////////////

AssetsWidget::AssetsWidget()
{
	m_window.reset(new sh::gui::Window(sh::math::Recti(0, 100, 250, 500)));
	const auto& viewport = sh::Device::GetInstance()->GetDriver()->GetViewPort();
	m_window->SetText("Assets");

	sh::gui::VerticalLayoutPtr windowLayout(new sh::gui::VerticalLayout());
	windowLayout->SetMargins(2, 2, 2, 2);

	auto guiMgr = sh::gui::GuiManager::GetInstance();
	auto buttonPrefab = guiMgr->GetStyle()->GetButton("TreeButton");
	buttonPrefab->SetMaximumSize(sh::math::Vector2u(20U));
	buttonPrefab->SetToggleable(true);
	//windowLayout->AddWidget(buttonPrefab->Clone());
	//windowLayout->AddWidget(buttonPrefab->Clone());
	//windowLayout->AddWidget(buttonPrefab->Clone());
	//windowLayout->AddWidget(buttonPrefab->Clone());

	m_window->SetLayout(windowLayout);

	sh::gui::GuiManager::GetInstance()->AddChild(m_window);

	//////////////////////////////////////////////////

	m_materialEditor.reset(new MaterialEditor());
	guiMgr->AddChild(m_materialEditor);


	struct Local
	{
		static void Parse(const sh::SPtr<sh::io::FileSystemComponent>& item, 
						  sh::SPtr<TreeWidget>& treeWidget,
						  sh::SPtr<TreeItem>& treeItem)
		{
			if (item->GetType() == sh::io::FileSystemComponent::Type::Folder)
			{
				auto folder = std::static_pointer_cast<sh::io::FolderInfo>(item);
				for (const auto& child : folder->children)
				{
					sh::SPtr<TreeItem> childItem = treeWidget->AddItem(child->name, treeItem.get());
					treeItem->AddChild(childItem);
					Parse(child, treeWidget, childItem);
				}
			}
		}
	};

	

	auto root = sh::io::FileSystem::GetInstance()->GetRoot();
	sh::SPtr<TreeWidget> treeWidget(new TreeWidget());
	sh::SPtr<TreeItem> rootTreeItem = treeWidget->AddItem(root->name, nullptr);

	Local::Parse(root, treeWidget, rootTreeItem);

	windowLayout->AddWidget(treeWidget);

	treeWidget->itemToggled.Connect(std::bind(&AssetsWidget::OnTreeItemToggled, this,
		std::placeholders::_1, std::placeholders::_2));

	
}

AssetsWidget::~AssetsWidget()
{
}

void AssetsWidget::OnTreeItemToggled(const sh::String& name, bool toggled)
{
	size_t pos = name.find_last_of('.');
	auto extension = name.substr(pos + 1);

	if (extension == "mat" && toggled)
	{
		auto material = sh::Device::GetInstance()->GetResourceManager()->GetMaterial(name);
		m_materialEditor->SetMaterial(material);
	}
	else
	{
		m_materialEditor->SetMaterial(nullptr);
	}
}