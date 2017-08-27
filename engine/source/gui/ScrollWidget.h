#ifndef SHADOW_SCROLL_WIDGET_INCLUDE
#define SHADOW_SCROLL_WIDGET_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{
	
	class ScrollWidget : public Widget
	{
	public:
		ScrollWidget(); 

		virtual ~ScrollWidget();

		virtual void Render(video::Painter* painter) override;
		virtual void SetPosition(s32 x, s32 y) override;
		virtual void SetSize(const math::Vector2u& size) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;
		virtual void UpdateLayout() override;

	private:
		math::Recti m_fullRect;
	};

} // gui

} // sh

#endif