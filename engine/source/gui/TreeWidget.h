#ifndef SHADOW_EDITOR_TREE_WIDGET_INCLUDE
#define SHADOW_EDITOR_TREE_WIDGET_INCLUDE

#include "Button.h"
#include "ScrollWidget.h"

namespace sh
{

namespace gui
{

	class SHADOW_API TreeExpandButton : public Button
	{
	public:
		TreeExpandButton();
		virtual ~TreeExpandButton();

		virtual void Render(video::Painter* painter) override;
	};

	class SHADOW_API TreeItem : public Button
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
		virtual void OnPressed();
        virtual void OnDoubleClicked();

		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;
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

	class SHADOW_API TreeWidget : public ScrollWidget
	{
        SHADOW_OBJECT(TreeWidget)
	public:
		TreeWidget();
        
        static void RegisterObject();
		void AddItem(SPtr<TreeItem> item);
		virtual void SetSelectedItem(TreeItem* item);
		TreeItem* GetSelectedItem() { return m_selectedItem; }

		virtual void Render(sh::video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;
		virtual void UpdateLayout() override;
		virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;

	private:
		TreeItem* m_selectedItem = nullptr;
	};

} // gui

} // sh



#endif
