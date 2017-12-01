#ifndef SHADOW_WINDOW_INCLUDE
#define SHADOW_WINDOW_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class SHADOW_API Window : public Widget
	{
		friend class Style;
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
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;

		virtual bool ProcessEvent(GUIEvent& ev) override;

		bool IsMovable() const { return m_isMovable; }
		void SetMovable(bool movable) { m_isMovable = movable; }

		bool IsClosable() const { return m_isClosable; }
		void SetClosable(bool closable) { m_isClosable = closable; }

		virtual void UpdateLayout() override;

		void Close();

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

		ButtonPtr m_closeButton;

		math::Recti m_barRect;
		math::Recti m_inRect;

		u32 m_barWidth = 25U;
		bool m_isMovable = true;
		bool m_isClosable = false;
	};

} // gui

} // sh

#endif