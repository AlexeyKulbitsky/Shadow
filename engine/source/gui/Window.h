#ifndef SHADOW_WINDOW_INCLUDE
#define SHADOW_WINDOW_INCLUDE

#include "GuiElement.h"

namespace sh
{

namespace gui
{

	class Window : public GuiElement
	{
	public:
		Window(const math::Rectu& rect, const SpritePtr& sprite);

		void SetText(const String& text);

		virtual void GetGeometry(GuiBatchData& data) override;
		virtual void GetTextGeometry(GuiBatchData& data) override;

		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;

	private:
		SpritePtr m_sprite;
		bool m_dragStarted = false;
		math::Vector2i m_startPos;

		TextPtr m_text;
	};

} // gui

} // sh

#endif