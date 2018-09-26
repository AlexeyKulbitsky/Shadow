#include "HierarchyWidget.h"

#include "EntityTreeItem.h"

//////////////////////////////////////////////////////////////////////////////////////

HeirarchyTreeItem::HeirarchyTreeItem(TreeItem* parent, sh::Entity* entity)
	:TreeItem(entity->GetName(), parent)
{
	m_entity = entity;
}

//////////////////////////////////////////////////////////////////////////////////////

void HeirarchyTreeItem::OnDoubleClick(sh::s32 x, sh::s32 y)
{
	if (m_treeWidget->GetSelectedItem() == this)
	{

	}
	else
	{

	}
}

//////////////////////////////////////////////////////////////////////////////////////

HierarchyTreeWidget::HierarchyTreeWidget()
{
}

//////////////////////////////////////////////////////////////////////////////////////

void HierarchyTreeWidget::SetSelectedItem(sh::gui::TreeItem* item)
{
	sh::gui::TreeWidget::SetSelectedItem(item);
	if (item)
	{
		auto hierarchyItem = static_cast<EntityTreeItem*>(item);
		auto entity = hierarchyItem->GetEntity();
		m_hierarchyWidget->OnEntitySelected(entity);
	}
	else
	{
		m_hierarchyWidget->OnEntitySelected(nullptr);
	}
}

//////////////////////////////////////////////////////////////////////////////////////

HierarchyWidget::HierarchyWidget()
	: sh::gui::Window(sh::math::Rect(0, 50, 250, 400))
{
	SetText("Hierarchy");
	SetMovable(false);
	m_layout.reset(new sh::gui::VerticalLayout());
	m_layout->SetMargins(2, 2, 2, 2);
	SetLayout(m_layout);

	m_treeWidget.reset(new HierarchyTreeWidget());
	m_treeWidget->SetHierarchyWidget(this);
	m_layout->AddWidget(m_treeWidget);
}

HierarchyWidget::~HierarchyWidget()
{
}

void HierarchyWidget::AddEntity(sh::Entity* entity, sh::gui::TreeItem* parent/* = nullptr*/)
{
	//sh::SPtr<HeirarchyTreeItem> item(new HeirarchyTreeItem(nullptr, entity));
	//m_treeWidget->AddItem(item);

    sh::SPtr<EntityTreeItem> item(new EntityTreeItem(parent, entity));
    m_treeWidget->AddItem(item);
    
    const auto groupName = sh::ObjectFactory::GetInstance()->GetGroupForObject(entity);
    if (groupName == "UI")
    {
        auto widget = static_cast<sh::gui::Widget*>(entity);
        auto layout = widget->GetLayout();
        if (layout)
        {
            sh::SPtr<EntityTreeItem> layoutItem(new EntityTreeItem(item.get(), layout.get()));
            m_treeWidget->AddItem(layoutItem);
            
            const auto itemsCount = layout->GetItemsCount();
            for (size_t i = 0; i < itemsCount; ++i)
            {
                const auto& childWidget = layout->GetWidget(i);
                if (childWidget)
                {
                    AddEntity(childWidget.get(), layoutItem.get());
                }
            }
        }
    }
}

void HierarchyWidget::SetSelectedEntity(sh::Entity* entity)
{
	if (entity)
	{
		const auto itemsCount = m_treeWidget->GetLayout()->GetItemsCount();
		for (size_t i = 0; i < itemsCount; ++i)
		{
			//auto hierarchyItem = static_cast<HeirarchyTreeItem*>(m_treeWidget->GetLayout()->GetWidget(i).get());
			auto hierarchyItem = static_cast<EntityTreeItem*>(m_treeWidget->GetLayout()->GetWidget(i).get());
			auto itemEntity = hierarchyItem->GetEntity();
			if (itemEntity == entity)
			{
				m_treeWidget->SetSelectedItem(hierarchyItem);
				return;
			}
		}
		m_treeWidget->SetSelectedItem(nullptr);
	}
	else
	{
		m_treeWidget->SetSelectedItem(nullptr);
	}
}

bool HierarchyWidget::ProcessEvent(sh::gui::GUIEvent& ev)
{
    if (sh::gui::Window::ProcessEvent(ev))
    {
        return true;
    }
    if(ev.type == sh::gui::EventType::PointerUp && m_inRect.IsPointInside(ev.x, ev.y))
    {
        sh::gui::MenuPtr menu(new sh::gui::Menu());
        menu->AddItem("Create");
        
        sh::gui::MenuPtr objectsMenu(new sh::gui::Menu());
        objectsMenu->AddItem("Entity");
        auto uiElementsList = sh::ObjectFactory::GetInstance()->GetObjectTypesForGroup("UI");
        for (const auto& uiELementType : uiElementsList)
        {
            objectsMenu->AddItem(uiELementType);
        }
        menu->AddSubmenu("Create", objectsMenu);
        menu->SetPosition(ev.x, ev.y);
        menu->itemSelected.Connect(std::bind(&HierarchyWidget::OnMenuItemSelected, this, std::placeholders::_1));
        sh::gui::GuiManager::GetInstance()->SetFocusWidget(menu);
        return true;
    }
    return false;
}

void HierarchyWidget::OnMenuItemSelected(const sh::String& itemName)
{
    if (itemName == "Entity")
    {
        sh::Entity* entity = new sh::Entity();
        entity->SetName("New object");
        sh::scene::SceneManager* sceneMgr = sh::Device::GetInstance()->GetSceneManager();
        sceneMgr->AddEntity(entity);
        AddEntity(entity);
    }
    else
    {
        auto uiElementsList = sh::ObjectFactory::GetInstance()->GetObjectTypesForGroup("UI");
        auto findResult = std::find(uiElementsList.begin(), uiElementsList.end(), itemName);
        if (findResult != uiElementsList.end())
        {
            auto object = sh::ObjectFactory::GetInstance()->CreateObject(*findResult);
            sh::gui::Widget* widgetRawPtr = static_cast<sh::gui::Widget*>(object);
            sh::SPtr<sh::gui::Widget> widget(widgetRawPtr);
            widget->SetPosition(400, 300);
            sh::gui::GuiManager::GetInstance()->AddChild(widget);
            AddEntity(widgetRawPtr);
        }
    }
}
