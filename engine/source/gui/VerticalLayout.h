#ifndef SHADOW_VERTICAL_LAYOUT_INCLUDE
#define SHADOW_VERTICAL_LAYOUT_INCLUDE

#include "Layout.h"

namespace sh
{

namespace gui
{

	class SHADOW_API VerticalLayout : public Layout
	{
        SHADOW_OBJECT(VerticalLayout)
	public:
        VerticalLayout();
        
        static void RegisterObject();
        
		virtual void AddWidget(const WidgetPtr& widget) override;
		virtual void RemoveWidget(const WidgetPtr& widget) override;
		virtual void AddLayout(const LayoutPtr& layout) override;
		virtual void SetSize(const math::Vector2Int& size) override;
		virtual void SetWidth(s32 width) override;
		virtual void SetHeight(s32 height) override;

	public:
		virtual void Resize(const math::Rect& rect) override;
	};

} // gui

} // sh

#endif
