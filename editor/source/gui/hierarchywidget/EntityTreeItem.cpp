#include "EntityTreeItem.h"

EntityTreeItem::EntityTreeItem(sh::gui::TreeItem* parent, sh::Entity* entity)
	: sh::gui::TreeItem(entity->GetName(), parent)
	, m_entity(entity)
{

}

EntityTreeItem::~EntityTreeItem()
{

}

void EntityTreeItem::OnPressed()
{

}

void EntityTreeItem::OnContextMenu(sh::s32 x, sh::s32 y)
{
	sh::gui::MenuPtr menu(new sh::gui::Menu());
	menu->AddItem("Add component");
	menu->SetPosition(x, y);

	sh::gui::MenuPtr componentsMenu(new sh::gui::Menu());
	auto componentList = sh::ObjectFactory::GetInstance()->GetObjectTypesForGroup("Components");
	for (const auto& componentType : componentList)
	{
		componentsMenu->AddItem(componentType);
	}
	menu->AddSubmenu("Add component", componentsMenu);
	//menu->itemSelected.Connect(std::bind(&MaterialTreeItem::OnMenuItemSelected, this, std::placeholders::_1));
	sh::gui::GuiManager::GetInstance()->SetFocusWidget(menu);
}

void EntityTreeItem::OnEdit(sh::s32 x, sh::s32 y)
{

}