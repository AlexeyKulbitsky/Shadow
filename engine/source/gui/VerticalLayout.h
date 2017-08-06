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
		virtual void SetSize(const math::Vector2u& size) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;

	public:
		virtual void Resize(const math::Rectu& rect) override;
	};

} // gui

} // sh

#endif