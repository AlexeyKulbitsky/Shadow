#ifndef SHADOW_LINE_EDIT_INCLUDE
#define SHADOW_LINE_EDIT_INCLUDE

#include "Text.h"

namespace sh
{

namespace gui
{

	class SHADOW_API LineEdit : public Text
	{
        SHADOW_OBJECT(LineEdit)
	public:
		enum State : u32
		{
			Default = 0U,
			Edit,

			Count
		};

		LineEdit();
		LineEdit(const SpritePtr& defaultSprite,
				const SpritePtr& editSprite);

		LineEdit(const math::Rect& rect, 
			   const SpritePtr& defaultSprite, 
			   const SpritePtr& editSprite);
        
        static void RegisterObject();

		void SetState(State state);
		virtual void Render(video::Painter* painter) override;
		virtual void RenderBackground(video::Painter* painter) override;
		virtual void RenderText(video::Painter* painter) override;
		virtual bool ProcessEvent(GUIEvent& ev) override;

		Event<void, const String&> OnTextChanged;
		Event<void, const String&> OnEditingFinished;

	protected:
		virtual void UpdateIfDirty();

	protected:
		std::array<SpritePtr, State::Count> m_sprites;
		State m_state = State::Default;

		bool m_dirty = false;
		//bool m_inFocus = false;

		static math::Rect s_cursorRect;
		static size_t s_cursorPos;
	};

} // gui

} // sh

#endif
