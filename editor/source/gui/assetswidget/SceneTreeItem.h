#ifndef SHADOW_SCENE_TREE_ITEM_INCLUDE
#define SHADOW_SCENE_TREE_ITEM_INCLUDE

#include <Shadow.h>

class SceneTreeItem : public sh::gui::TreeItem
{
public:
    SceneTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem);
    virtual ~SceneTreeItem();
    void OnDoubleClicked() override final;

protected:
    void OnEdit(sh::s32 x, sh::s32 y) override final;

private:
    sh::io::FileSystemComponent* m_item = nullptr;
};

#endif