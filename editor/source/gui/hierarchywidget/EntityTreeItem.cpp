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
	sh::gui::TreeItem::OnPressed();
}

void EntityTreeItem::OnContextMenu(sh::s32 x, sh::s32 y)
{
	auto components = m_entity->GetComponents();

	sh::gui::MenuPtr menu(new sh::gui::Menu());
	menu->AddItem("Add component");
	menu->SetPosition(x, y);

	sh::gui::MenuPtr componentsMenu(new sh::gui::Menu());
	auto componentList = sh::ObjectFactory::GetInstance()->GetObjectTypesForGroup("Components");

	bool componentExists = false;
	for (const auto& componentType : componentList)
	{
		componentExists = false;
		for (auto component : components)
		{
			if (!component)
				continue;

			if (component->GetTypeName() == componentType)
			{
				componentExists = true;
				break;
			}
		}

		if (!componentExists)
			componentsMenu->AddItem(componentType);
	}
	menu->AddSubmenu("Add component", componentsMenu);

	menu->itemSelected.Connect(std::bind(&EntityTreeItem::OnMenuItemSelected, this, std::placeholders::_1));
	sh::gui::GuiManager::GetInstance()->SetFocusWidget(menu);
}

void EntityTreeItem::OnEdit(sh::s32 x, sh::s32 y)
{

}

void EntityTreeItem::OnMenuItemSelected(const sh::String& itemName)
{
	if (!m_entity)
		return;

	auto object = sh::ObjectFactory::GetInstance()->CreateObject(itemName);
	if (!object)
		return;

	auto component = static_cast<sh::Component*>(object);
	component->SetParentEntity(m_entity);
	m_entity->AddComponent(component);
}