#ifndef SHADOW_MODEL_TREE_ITEM_INCLUDE
#define SHADOW_MODEL_TREE_ITEM_INCLUDE

#include <Shadow.h>

class ModelTreeItem : public sh::gui::TreeItem
{
public:
	ModelTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem);
	virtual ~ModelTreeItem();
	virtual void OnToggled(bool toggled) override;

private:
	sh::io::FileSystemComponent* m_item = nullptr;
};

#endif
