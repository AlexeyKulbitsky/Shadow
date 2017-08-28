#ifndef SHADOW_VERTICAL_LAYOUT_INCLUDE
#define SHADOW_VERTICAL_LAYOUT_INCLUDE

#include "Layout.h"

namespace sh
{

namespace gui
{

	class VerticalLayout : public Layout
	{
	public:
		VerticalLayout() {}
		virtual void AddWidget(const WidgetPtr& widget) override;
		virtual void RemoveWidget(const WidgetPtr& widget) override;
		virtual void AddLayout(const LayoutPtr& layout) override;
		virtual void SetSize(const math::Vector2i& size) override;
		virtual void SetWidth(s32 width) override;
		virtual void SetHeight(s32 height) override;

	public:
		virtual void Resize(const math::Recti& rect) override;
	};

} // gui

} // sh

#endif