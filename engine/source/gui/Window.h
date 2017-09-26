#ifndef SHADOW_WINDOW_INCLUDE
#define SHADOW_WINDOW_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class Window : public Widget
	{
	public:
		Window(const math::Recti& rect);
		Window(const SpritePtr& outSprite, const SpritePtr& inSprite, const SpritePtr& barSprite);

		void SetText(const String& text);

		virtual void SetRect(const math::Recti& rect) override;
		virtual void SetPosition(s32 x, s32 y) override;
		virtual void SetSize(const math::Vector2i& size) override;
		virtual void SetWidth(s32 width) override;
		virtual void SetHeight(s32 height) override;


		virtual void Render(video::Painter* painter) override;

		virtual bool ProcessEvent(GUIEvent& ev) override;

		bool IsMovable() const { return m_isMovable; }
		void SetMovable(bool movable) { m_isMovable = movable; }

		virtual void UpdateLayout() override;

	private:
		void InternalUpdate();

	private:
		SpritePtr m_sprite;
		bool m_dragStarted = false;
		math::Vector2i m_startPos;

		TextPtr m_text;

		SpritePtr m_outSprite;
		SpritePtr m_inSprite;
		SpritePtr m_barSprite;

		math::Recti m_barRect;
		math::Recti m_inRect;

		u32 m_barWidth = 25U;
		bool m_isMovable = true;
	};

} // gui

} // sh

#endif