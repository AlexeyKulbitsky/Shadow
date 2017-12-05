#ifndef ENTITY_TREE_ITEM_INCLUDE
#define ENTITY_TREE_ITEM_INCLUDE

#include <Shadow.h>

class EntityTreeItem : public sh::gui::TreeItem
{
public:
	EntityTreeItem(sh::gui::TreeItem* parent, sh::Entity* entity);
	virtual ~EntityTreeItem();
	virtual void OnPressed() override;

	sh::Entity* GetEntity() { return m_entity; }

protected:
	virtual void OnContextMenu(sh::s32 x, sh::s32 y) override;
	virtual void OnEdit(sh::s32 x, sh::s32 y) override;

	void OnMenuItemSelected(const sh::String& itemName);

private:
	sh::Entity* m_entity = nullptr;
};

#endif