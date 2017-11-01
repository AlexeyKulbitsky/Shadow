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
		// Set the same color for all corners
		void SetColor(const math::Vector4f& color);
		// Set different color for each corner
		void SetColor(const math::Vector4f& downLeft, const math::Vector4f& upperLeft, const math::Vector4f& upperRight, const math::Vector4f& downRight);

		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override {}

	private:
		SpritePtr m_sprite;
		std::array<math::Vector4f, 4U> m_colors;
	};

} // gui

} // sh

#endif