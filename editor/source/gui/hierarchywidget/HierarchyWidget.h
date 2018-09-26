#ifndef SHADOW_EDITOR_HIERARCHY_WIDGET_INCLUDE
#define SHADOW_EDITOR_HIERARCHY_WIDGET_INCLUDE

#include <Shadow.h>

//////////////////////////////////////////////////////////////////////////////////////

class HeirarchyTreeItem : public sh::gui::TreeItem
{
public:
	HeirarchyTreeItem(sh::gui::TreeItem* parent, sh::Entity* entity);

	sh::Entity* GetEntity() { return m_entity; }

private:
	void OnDoubleClick(sh::s32 x, sh::s32 y);

private:
	sh::Entity* m_entity = nullptr;
};

//////////////////////////////////////////////////////////////////////////////////////
class HierarchyWidget;

class HierarchyTreeWidget : public sh::gui::TreeWidget
{
public:
	HierarchyTreeWidget();
	void SetHierarchyWidget(HierarchyWidget* widget) { m_hierarchyWidget = widget; }
	virtual void SetSelectedItem(sh::gui::TreeItem* item) override;

private:
	HierarchyWidget* m_hierarchyWidget = nullptr;
};

//////////////////////////////////////////////////////////////////////////////////////

class HierarchyWidget : public sh::gui::Window
{
public:
	HierarchyWidget();
	~HierarchyWidget();

    void AddEntity(sh::Entity* entity, sh::gui::TreeItem* parent = nullptr);
	void SetSelectedEntity(sh::Entity* entity);

    virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;
    
	sh::Event<void, sh::Entity*> OnEntitySelected;

private:
	void SelectEntity(sh::u32 index);
    void OnMenuItemSelected(const sh::String& itemName);

private:
	sh::gui::VerticalLayoutPtr m_layout;
	sh::gui::ButtonPtr m_currentSelectedItem;

	sh::SPtr<HierarchyTreeWidget> m_treeWidget;
};

#endif
