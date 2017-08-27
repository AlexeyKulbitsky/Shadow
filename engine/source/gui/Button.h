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
		enum State : u32
		{
			Pressed,
			Released,
			Hovered,

			Count
		};

		Button(const math::Recti rect);
		Button(const SpritePtr& defaultSprite,
				const SpritePtr& pressedSprite,
				const SpritePtr& hoveredSprite);
		Button(const math::Recti& rect, 
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
		virtual void Render(video::Painter* painter) override;
		virtual void SetPosition(s32 x, s32 y) override;
		virtual void SetSize(const math::Vector2u& size) override;
		virtual void SetWidth(u32 width) override;
		virtual void SetHeight(u32 height) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;

		u32 GetHeight() const;
		u32 GetWidth() const;
		const math::Vector2i& GetPosition() const;

		Event<void, const ButtonPtr&> OnPress;
		Event<void, const ButtonPtr&> OnRelease;
		Event<void, const ButtonPtr&> OnHover;
		Event<void, bool, const ButtonPtr&> OnToggle;

	private:
		std::array<SpritePtr, State::Count> m_sprites;
		State m_state = State::Released;

		TextPtr m_text;

		bool m_toggleable = false;
		bool m_toggled = false;
	};

} // gui

} // sh

#endif