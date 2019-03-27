#include "SceneTreeItem.h"
#include "AssetsWidget.h"

SceneTreeItem::SceneTreeItem(sh::gui::TreeItem* parent, sh::io::FileSystemComponent* fsItem)
    : TreeItem(fsItem->name, parent)
{
    m_item = fsItem;

    auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("image");
    auto clonedIcon = icon->Clone();
    clonedIcon->SetMaximumWidth(15);
    m_layout->InsertWidget(0U, clonedIcon);
}

SceneTreeItem::~SceneTreeItem()
{

}

void SceneTreeItem::OnDoubleClicked()
{
    sh::scene::SceneManager* sceneMgr = sh::Device::GetInstance()->GetSceneManager();
    sceneMgr->LoadScene(m_item->name.c_str());
    sh::u32 entitiesCount = sceneMgr->GetEntitiesCount();
    
    //for (sh::u32 i = 0U; i < entitiesCount; ++i)
    //{
    //    auto entity = sceneMgr->GetEntity(i);
    //    m_hierarchyWidget->AddEntity(entity);
    //}
}

void SceneTreeItem::OnEdit(sh::s32, sh::s32)
{
    sh::scene::SceneManager* sceneMgr = sh::Device::GetInstance()->GetSceneManager();
    sceneMgr->LoadScene(m_item->name.c_str());
    sh::u32 entitiesCount = sceneMgr->GetEntitiesCount();
}
