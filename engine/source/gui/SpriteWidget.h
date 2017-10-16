#ifndef SHADOW_SPRITE_WIDGET_INCLUDE
#define SHADOW_SPRITE_WIDGET_INCLUDE

#include "Widget.h"
#include "Sprite.h"

namespace sh
{

namespace gui
{

	class SpriteWidget : public Widget
	{
	public:
		SpriteWidget();

		SpriteWidgetPtr Clone();
		void SetSprite(const SpritePtr& sprite) { m_sprite = sprite; }
		const SpritePtr& GetSprite() const { return m_sprite; }

		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override {}

	private:
		SpritePtr m_sprite;
	};

} // gui

} // sh

#endif