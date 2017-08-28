#ifndef SHADOW_HORIZONTAL_LAYOUT_INCLUDE
#define SHADOW_HORIZONTAL_LAYOUT_INCLUDE

#include "Layout.h"

namespace sh
{

namespace gui
{

	class HorizontalLayout : public Layout
	{
	public:
		virtual void AddWidget(const WidgetPtr& widget) override;
		virtual void RemoveWidget(const WidgetPtr& widget) override;
		virtual void AddLayout(const LayoutPtr& layout) override;
		virtual void SetSize(const math::Vector2i& size) override;
		virtual void SetWidth(s32 width) override;
		virtual void SetHeight(s32 height) override;

	protected:
		virtual void Resize(const math::Recti& rect) override;
		
	};

} // gui

} // sh

#endif