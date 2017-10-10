#ifndef SHADOW_MATERIAL_TREE_ITEM_INCLUDE
#define SHADOW_MATERIAL_TREE_ITEM_INCLUDE

#include <Shadow.h>

class MaterialTreeItem : public sh::gui::TreeItem
{
public:
	MaterialTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem);
	virtual ~MaterialTreeItem();
	virtual void OnPressed() override;

protected:
	virtual void OnContextMenu(sh::s32 x, sh::s32 y) override;
	virtual void OnEdit(sh::s32 x, sh::s32 y) override;

private:
	void OnMenuItemSelected(const sh::String& itemName);

private:
	sh::io::FileSystemComponent* m_item = nullptr;
};

#endif
