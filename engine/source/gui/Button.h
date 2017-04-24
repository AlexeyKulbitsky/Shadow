#ifndef SHADOW_BUTTON_INCLUDE
#define SHADOW_BUTTON_INCLUDE

#include "GuiElement.h"

namespace sh
{

namespace gui
{

	class Button : public GuiElement
	{
	public:
		Button(const math::Rectf rect);

		virtual void SetPosition(u32 x, u32 y) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;

		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;

		Event<void> pressed;
		Event<void> released;

	private:
		math::Rectf m_rect;
		u32 m_offset = 0U;
	};

} // gui

} // sh

#endif