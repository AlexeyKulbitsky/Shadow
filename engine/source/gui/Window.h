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
		Window(const math::Rectu& rect);
		Window(const SpritePtr& outSprite, const SpritePtr& inSprite, const SpritePtr& barSprite);

		void SetText(const String& text);

		virtual void SetPosition(u32 x, u32 y) override;
		virtual void GetGeometry(GuiBatchData& data) override;
		virtual void GetTextGeometry(GuiBatchData& data) override;
		virtual void Render(video::Painter* painter) override;

		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;

		bool IsMovable() const { return m_isMovable; }
		void SetMovable(bool movable) { m_isMovable = movable; }

	protected:
		virtual void UpdatePosition() override;
		virtual void UpdateUV(const math::Vector2f& leftUp, const math::Vector2f& rightDown) override;
		virtual void UpdateColor(const math::Vector4f& color) override;

	public:
		virtual void UpdateLayout() override;

	private:
		SpritePtr m_sprite;
		bool m_dragStarted = false;
		math::Vector2i m_startPos;

		TextPtr m_text;

		SpritePtr m_outSprite;
		SpritePtr m_inSprite;
		SpritePtr m_barSprite;

		math::Rectu m_barRect;
		math::Rectu m_inRect;

		u32 m_barWidth = 25U;
		bool m_isMovable = true;
	};

} // gui

} // sh

#endif