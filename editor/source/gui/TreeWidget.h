#ifndef SHADOW_EDITOR_TREE_WIDGET_INCLUDE
#define SHADOW_EDITOR_TREE_WIDGET_INCLUDE

#include <Shadow.h>

class TreeExpandButton : public sh::gui::Button
{
public:
	TreeExpandButton();
	virtual ~TreeExpandButton();

	virtual void Render(sh::video::Painter* painter) override;
};

class TreeItem : public sh::gui::Button
{
	friend class TreeWidget;
public:
	TreeItem(const sh::String& name, TreeItem* parent);
	virtual ~TreeItem();

	TreeItem* GetParent() { return m_parent; }
	void AddChild(const sh::SPtr<TreeItem>& child);
	sh::s32 GetOffset() const { return m_offset; }
	bool IsExpanded() const { return m_expanded; }
	void SetExpanded(bool expanded);
	virtual void OnToggled(bool toggled);

	virtual bool ProcessEvent(sh::gui::GUIEvent& ev);

protected:
	virtual void OnContextMenu(sh::s32 x, sh::s32 y) { }
	virtual void OnEdit(sh::s32 x, sh::s32 y) { }

protected:
	sh::s32 m_offset = 0;
	bool m_expanded = true;
	TreeItem* m_parent = nullptr;
	TreeWidget* m_treeWidget = nullptr;
	std::vector<sh::SPtr<TreeItem>> m_children;
};

class TreeWidget : public sh::gui::ScrollWidget
{
public:
	TreeWidget();
	void AddItem(sh::SPtr<TreeItem> item);

	virtual void Render(sh::video::Painter* painter) override;
	virtual void UpdateLayout() override;
	virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;
};

#endif