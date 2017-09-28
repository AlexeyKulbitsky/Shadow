#ifndef SHADOW_LAYOUT_INCLUDE
#define SHADOW_LAYOUT_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{
	class LayoutItem
	{
	public:
		LayoutItem() {}
		virtual const WidgetPtr& GetWidget() const { return m_widget; }
		virtual void SetSize(const math::Vector2i& size) = 0;
		virtual void SetWidth(s32 width) = 0;
		virtual void SetHeight(s32 height) = 0;
		virtual const math::Vector2i GetSize() const = 0;
		virtual s32 GetWidth() const = 0;
		virtual s32 GetHeight() const = 0;
		virtual void Resize(const math::Recti& rect) {}
		virtual void Render(video::Painter* painter) {}
		virtual bool ProcessEvent(GUIEvent& ev) { return false; }

	protected:
		WidgetPtr m_widget;
	};

	//////////////////////////////////////////////////////////////////////

	class WidgetItem : public LayoutItem
	{
	public:
		WidgetItem(const WidgetPtr& widget) { m_widget = widget; }
		virtual const WidgetPtr& GetWidget() const override { return m_widget; }

		virtual void SetSize(const math::Vector2i& size) override { m_widget->SetSize(size); }
		virtual void SetWidth(s32 width) override { m_widget->SetWidth(width); }
		virtual void SetHeight(s32 height) override { m_widget->SetHeight(height); }
		virtual const math::Vector2i GetSize() const override { return m_widget->GetRect().GetSize(); }
		virtual s32 GetWidth() const  override { return m_widget->GetRect().GetWidth(); }
		virtual s32 GetHeight() const  override { return m_widget->GetRect().GetHeight(); }
		virtual void Resize(const math::Recti& rect) override;
		virtual void Render(video::Painter* painter) { m_widget->Render(painter); }
		virtual bool ProcessEvent(GUIEvent& ev) { return m_widget->ProcessEvent(ev); }
	};
	

	//////////////////////////////////////////////////////////////////////

	class Layout : public LayoutItem
	{
		friend class Widget;
		friend class Window;
		friend class ScrollWidget;
	public:
		Layout() {}
		void SetParent(Widget* parent) { m_parent = parent; }
		virtual void AddWidget(const WidgetPtr& widget);
		virtual void InsertWidget(u32 index, const WidgetPtr& widget);
		virtual void RemoveWidget(const WidgetPtr& widget);
		virtual void Clear();
		virtual void AddLayout(const LayoutPtr& layout);

		virtual void SetSize(const math::Vector2i& size) override;
		virtual void SetWidth(s32 width) override;
		virtual void SetHeight(s32 height) override;
		virtual const math::Vector2i GetSize() const override { return m_rect.GetSize(); }
		virtual s32 GetWidth() const  override { return m_rect.GetWidth(); }
		virtual s32 GetHeight() const  override { return m_rect.GetHeight(); }
		// Sets spacing between elements
		virtual void SetSpacing(s32 spacing) { m_spacing = spacing; }
		// Returns actual spacing between elements
		virtual s32 GetSpacing() const { return m_spacing; }

		void SetMargins(u32 top, u32 right, u32 bottom, u32 left);
		const u32 GetTopMargin() const { return m_topMargin; }
		const u32 GetBottomMargin() const { return m_bottomMargin; }
		const u32 GetRightMargin() const { return m_rightMargin; }
		const u32 GetLeftMargin() const { return m_leftMargin; }

		// Actual rect of layout
		const math::Recti& GetRect() const { return m_rect; }


		virtual void Render(video::Painter* painter) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;
		virtual void Resize(const math::Recti& rect) override {}

		u32 GetItemsCount() const { return m_items.size(); }
		const LayoutItemPtr& GetItem(u32 index) { return m_items[index]; }
		const WidgetPtr& GetWidget(u32 index) { return m_items[index]->GetWidget(); }

	protected:
		std::vector<LayoutItemPtr> m_items;
		Widget* m_parent = nullptr;
		math::Recti m_rect;

		u32 m_topMargin = 0U;
		u32 m_rightMargin = 0U;
		u32 m_bottomMargin = 0U;
		u32 m_leftMargin = 0U;

		s32 m_spacing = 0;
	};

} // gui

} // sh

#endif