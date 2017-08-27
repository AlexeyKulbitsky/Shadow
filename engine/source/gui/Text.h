#ifndef SHADOW_TEXT_INCLUDE
#define SHADOW_TEXT_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class Text : public Widget
	{
	public:
		Text(); 
		Text(const math::Recti rect);
		Text(const String& text);
		Text(const math::Recti rect, const String& text);

		virtual ~Text();

		void SetText(const String& text);
		const String& GetText() const { return m_text; }

		virtual void Render(video::Painter* painter) override;
		virtual void SetPosition(s32 x, s32 y) override;
		virtual void SetSize(const math::Vector2u& size) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;

	protected:
		String m_text;
	};

} // gui

} // sh

#endif