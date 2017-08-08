#ifndef SHADOW_BUTTON_INCLUDE
#define SHADOW_BUTTON_INCLUDE

#include "Widget.h"

namespace sh
{

namespace gui
{

	class Button : public Widget
	{
	public:
		Button(const math::Rectu rect);
		Button(const SpritePtr& defaultSprite,
				const SpritePtr& pressedSprite,
				const SpritePtr& hoveredSprite);
		Button(const math::Rectu& rect, 
			   const SpritePtr& defaultSprite, 
			   const SpritePtr& pressedSprite,
			   const SpritePtr& hoveredSprite);
		Button();
		Button(const String& text);

		ButtonPtr Clone() const;

		void SetToggleable(bool toggleable) { m_toggleable = toggleable; }
		void SetText(const String& text);
		const String& GetText() const;
		void SetToggled(bool toggled);
		bool IsToggled() const { return m_toggled; }

		virtual void Load(const pugi::xml_node& node) override;
		virtual void GetGeometry(GuiBatchData& data) override;
		virtual void GetTextGeometry(GuiBatchData& data) override;
		virtual void SetPosition(u32 x, u32 y) override;
		virtual void SetSize(const math::Vector2u& size) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;
		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;

		u32 GetHeight() const;
		u32 GetWidth() const;
		const math::Vector2u& GetPosition() const;

		Event<void, const ButtonPtr&> OnPress;
		Event<void, const ButtonPtr&> OnRelease;
		Event<void, const ButtonPtr&> OnHover;
		Event<void, bool, const ButtonPtr&> OnToggle;

	private:
		SpritePtr m_pressedSprite;
		SpritePtr m_releasedSprite;
		SpritePtr m_hoveredSprite;

		TextPtr m_text;

		bool m_toggleable = false;
		bool m_toggled = false;
	};

} // gui

} // sh

#endif