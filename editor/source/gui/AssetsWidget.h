#ifndef SHADOW_EDITOR_ASSETS_WIDGET_INCLUDE
#define SHADOW_EDITOR_ASSETS_WIDGET_INCLUDE

#include <Shadow.h>

class TreeItem : public sh::gui::Widget
{
	friend class TreeWidget;
public:
	TreeItem(const sh::String& name, TreeItem* parent);
	~TreeItem();

	TreeItem* GetParent() { return m_parent; }
	void AddChild(const sh::SPtr<TreeItem>& child);
	sh::s32 GetOffset() const { return m_offset; }
	bool IsExpanded() const { return m_expanded; }
	void SetExpanded(bool expanded);// { m_expanded = expanded; }
	void SetVisibility(bool yes);
	void OnToggled(bool toggled);

private:
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
	sh::SPtr<TreeItem> AddItem(const sh::String& name, TreeItem* parent);

	virtual void Render(sh::video::Painter* painter) override;
	virtual void UpdateLayout() override;

private:
	sh::gui::VerticalLayoutPtr m_layout;
};

///////////////////////////////////////////////////////////////////////

class AssetsWidget
{
public:
	AssetsWidget();
	~AssetsWidget();

private:
	sh::gui::WindowPtr m_window;
};

#endif
