#ifndef SHADOW_TEXTURE_TREE_ITEM_INCLUDE
#define SHADOW_TEXTURE_TREE_ITEM_INCLUDE

#include <Shadow.h>

class TextureTreeItem : public sh::gui::TreeItem
{
public:
	TextureTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem);
	virtual ~TextureTreeItem();
	virtual void OnToggled(bool toggled) override;

private:
	sh::io::FileSystemComponent* m_item = nullptr;
};

#endif