#ifndef SHADOW_SPRITE_WIDGET_INCLUDE
#define SHADOW_SPRITE_WIDGET_INCLUDE

#include "Widget.h"
#include "Sprite.h"

namespace sh
{

namespace gui
{

	class SHADOW_API SpriteWidget : public Widget
	{
        SHADOW_OBJECT(SpriteWidget)
	public:
		SpriteWidget();
        
        static void RegisterObject();

		SpriteWidgetPtr Clone();
		void SetSprite(const SpritePtr& sprite) { m_sprite = sprite; }
		const SpritePtr& GetSprite() const { return m_sprite; }
		// Set the same color for all corners
		void SetColor(const math::Vector4& color);
		// Set different color for each corner
		void SetColor(const math::Vector4& downLeft, const math::Vector4& upperLeft, const math::Vector4& upperRight, const math::Vector4& downRight);

		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override {}

	private:
		SpritePtr m_sprite;
		std::array<math::Vector4, 4U> m_colors;
	};

} // gui

} // sh

#endif
