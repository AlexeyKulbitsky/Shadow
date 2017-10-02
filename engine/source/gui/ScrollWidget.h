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
		virtual void SetSize(const math::Vector2i& size) override;
		virtual void SetWidth(s32 width) override;
		virtual void SetHeight(s32 height) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;
		virtual void UpdateLayout() override;

	protected:
		void UpdateScrollBars();

	protected:
		math::Recti m_fullRect;
		s32 m_scrollSpeed = 15;
	};

} // gui

} // sh

#endif