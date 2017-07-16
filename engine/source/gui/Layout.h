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
		LayoutItem(const WidgetPtr& widget);
		const WidgetPtr& GetWidget() const;

		virtual void SetSize(const math::Vector2u& size) {}
		virtual void SetWidth(u32 width) {}
		virtual void SetHeight(u32 height) {}
		virtual void Resize(const math::Rectu& rect) {}
		virtual void GetGeometry(GuiBatchData& data) {}
		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) { return false; }

	private:
		WidgetPtr m_widget;
	};
	

	//////////////////////////////////////////////////////////////////////

	class Layout : public LayoutItem
	{
		friend class Widget;
	public:
		Layout() {}
		virtual void AddWidget(const WidgetPtr& widget);
		virtual void AddLayout(const LayoutPtr& layout);
		virtual void SetSize(const math::Vector2u& size) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;

		u32 GetItemsCount() const { return m_items.size(); }
		const LayoutItemPtr& GetItem(u32 index) { return m_items[index]; }
		const WidgetPtr& GetWidget(u32 index) { return m_items[index]->GetWidget(); }

		virtual void GetGeometry(GuiBatchData& data) override;
		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;

	protected:
		virtual void Resize(const math::Rectu& rect) override {}

	protected:
		std::vector<LayoutItemPtr> m_items;
	};

} // gui

} // sh

#endif