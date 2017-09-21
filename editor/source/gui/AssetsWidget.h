#ifndef SHADOW_EDITOR_ASSETS_WIDGET_INCLUDE
#define SHADOW_EDITOR_ASSETS_WIDGET_INCLUDE

#include "gui/materialview/MaterialEditor.h"

class TreeItem : public sh::gui::Widget
{
	friend class TreeWidget;
public:
	TreeItem(sh::io::FileSystemComponent* fsItem, TreeItem* parent);
	~TreeItem();

	TreeItem* GetParent() { return m_parent; }
	void AddChild(const sh::SPtr<TreeItem>& child);
	sh::s32 GetOffset() const { return m_offset; }
	bool IsExpanded() const { return m_expanded; }
	void SetExpanded(bool expanded);
	void OnToggled(bool toggled);

	virtual bool ProcessEvent(sh::gui::GUIEvent& ev);
	
private:
	void OnMenuItemSelected(const sh::String& itemName);

private:
	sh::s32 m_offset = 0;
	sh::io::FileSystemComponent* m_item = nullptr;
	bool m_expanded = true;
	TreeItem* m_parent = nullptr;
	TreeWidget* m_treeWidget = nullptr;
	std::vector<sh::SPtr<TreeItem>> m_children;
};

class TreeWidget : public sh::gui::ScrollWidget
{
public:
	TreeWidget();
	sh::SPtr<TreeItem> AddItem(sh::io::FileSystemComponent* fsItem, TreeItem* parent);

	virtual void Render(sh::video::Painter* painter) override;
	virtual void UpdateLayout() override;

	sh::Event<void, const sh::String&, bool> itemToggled;

private:
	//sh::gui::VerticalLayoutPtr m_layout;
};

///////////////////////////////////////////////////////////////////////

class AssetsWidget
{
public:
	AssetsWidget();
	~AssetsWidget();

	void OnTreeItemToggled(const sh::String& name, bool toggled);

private:
	sh::gui::WindowPtr m_window;

	sh::SPtr<MaterialEditor> m_materialEditor;
};

#endif
