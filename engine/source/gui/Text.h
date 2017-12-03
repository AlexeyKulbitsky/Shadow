#ifndef SHADOW_TEXT_INCLUDE
#define SHADOW_TEXT_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class SHADOW_API Text : public Widget
	{
	public:
		Text(); 
		Text(const math::Rect rect);
		Text(const String& text);
		Text(const math::Rect rect, const String& text);

		virtual ~Text();

		void SetText(const String& text);
		const String& GetText() const { return m_text; }

		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override {}
		virtual void RenderText(video::Painter* painter) override;
		virtual void SetRect(const math::Rect& rect) override;
		virtual void SetPosition(s32 x, s32 y) override;
		virtual void SetSize(const math::Vector2Int& size) override;
		virtual void SetWidth(s32 width) override;
		virtual void SetHeight(s32 height) override;

	protected:
		String m_text;
		std::vector<s32> m_glyphOffsets;
	};

} // gui

} // sh

#endif