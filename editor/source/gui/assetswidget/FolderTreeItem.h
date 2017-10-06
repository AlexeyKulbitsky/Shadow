#ifndef SHADOW_FOLDER_TREE_ITEM_INCLUDE
#define SHADOW_FOLDER_TREE_ITEM_INCLUDE

#include <Shadow.h>

class FolderTreeItem : public sh::gui::TreeItem
{
public:
	FolderTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem);
	virtual ~FolderTreeItem();

protected:
	void OnExpanded(bool expanded);
	virtual void OnContextMenu(sh::s32 x, sh::s32 y) override;

private:
	void OnMenuItemSelected(const sh::String& itemName);

private:
	sh::io::FileSystemComponent* m_item = nullptr;
};

#endif
