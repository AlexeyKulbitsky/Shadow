#ifndef SHADOW_BUTTON_INCLUDE
#define SHADOW_BUTTON_INCLUDE

#include "Widget.h"

namespace pugi
{
	class xml_node;
}

namespace sh
{

namespace gui
{

	class SHADOW_API Button : public Widget
	{
        SHADOW_OBJECT(Button)
	public:
		enum State : u32
		{
			Pressed,
			Released,
			Hovered,

			Count
		};

		Button(const math::Rect rect);
		Button(const SpritePtr& defaultSprite,
				const SpritePtr& pressedSprite,
				const SpritePtr& hoveredSprite);
		Button(const math::Rect& rect, 
			   const SpritePtr& defaultSprite, 
			   const SpritePtr& pressedSprite,
			   const SpritePtr& hoveredSprite);
		Button();
		Button(const String& text);
		
        static void RegisterObject();
        
		ButtonPtr Clone() const;

		void SetToggleable(bool toggleable) { m_toggleable = toggleable; }
        bool IsToggleable() const { return m_toggleable; }
		void SetText(const String& text);
		const String& GetText() const;
		void SetToggled(bool toggled);
		bool IsToggled() const { return m_toggled; }

		virtual void Load(const pugi::xml_node& node) override;
		virtual void Render(video::Painter* painter) override;
		virtual void CollectBatches(GuiLayerBatch& batch) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;
		virtual void SetRect(const math::Rect& rect) override;
		virtual void SetPosition(s32 x, s32 y) override;
		virtual void SetSize(const math::Vector2Int& size) override;
		virtual void SetWidth(s32 width) override;
		virtual void SetHeight(s32 height) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;

		u32 GetHeight() const;
		u32 GetWidth() const;
		const math::Vector2Int& GetPosition() const;

		SpritePtr GetSprite(State state) const;

		Event<void, const ButtonPtr&> OnPress;
		Event<void, const ButtonPtr&> OnRelease;
		Event<void, const ButtonPtr&> OnHover;
		Event<void, bool, const ButtonPtr&> OnToggle;
        Event<void, const ButtonPtr&> OnDoubleClick;

	protected:
		std::array<SpritePtr, State::Count> m_sprites;
		State m_state = State::Released;

		TextPtr m_text;

		bool m_toggleable = false;
		bool m_toggled = false;
	};

} // gui

} // sh

#endif
