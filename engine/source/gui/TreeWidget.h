#ifndef SHADOW_EDITOR_TREE_WIDGET_INCLUDE
#define SHADOW_EDITOR_TREE_WIDGET_INCLUDE

#include "Button.h"
#include "ScrollWidget.h"

namespace sh
{

namespace gui
{

	class TreeExpandButton : public Button
	{
	public:
		TreeExpandButton();
		virtual ~TreeExpandButton();

		virtual void Render(video::Painter* painter) override;
	};

	class TreeItem : public sh::gui::Button
	{
		friend class TreeWidget;
	public:
		TreeItem(const String& name, TreeItem* parent);
		virtual ~TreeItem();

		TreeItem* GetParent() { return m_parent; }
		void AddChild(const SPtr<TreeItem>& child);
		s32 GetOffset() const { return m_offset; }
		bool IsExpanded() const { return m_expanded; }
		void SetExpanded(bool expanded);
		virtual void OnToggled(bool toggled);

		virtual void Render(video::Painter* painter) override;
		virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;

	protected:
		virtual void OnContextMenu(s32 x, s32 y) { }
		virtual void OnEdit(s32 x, s32 y) { }

	protected:
		s32 m_offset = 0;
		bool m_expanded = true;
		TreeItem* m_parent = nullptr;
		TreeWidget* m_treeWidget = nullptr;
		std::vector<sh::SPtr<TreeItem>> m_children;
	};

	class TreeWidget : public ScrollWidget
	{
	public:
		TreeWidget();
		void AddItem(SPtr<TreeItem> item);
		virtual void SetSelectedItem(TreeItem* item);

		virtual void Render(sh::video::Painter* painter) override;
		virtual void UpdateLayout() override;
		virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;

	private:
		TreeItem* m_selectedItem = nullptr;
	};

} // gui

} // sh



#endif