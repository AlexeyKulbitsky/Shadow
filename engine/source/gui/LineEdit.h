#ifndef SHADOW_LINE_EDIT_INCLUDE
#define SHADOW_LINE_EDIT_INCLUDE

#include "Text.h"

namespace sh
{

namespace gui
{

	class LineEdit : public Text
	{
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

		LineEdit(const math::Rectu& rect, 
			   const SpritePtr& defaultSprite, 
			   const SpritePtr& editSprite);

		virtual void Render(video::Painter* painter) override;

		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;
		virtual bool ProcessKeyboardInput(KeyboardEventType type, KeyCode code) override;

		Event<void, const String&> OnTextChanged;

	protected:
		virtual void UpdateIfDirty();
		//static void UpdateCursorGeometry();

	protected:
		std::array<SpritePtr, State::Count> m_sprites;
		State m_state = State::Default;

		bool m_dirty = false;
		bool m_inFocus = false;

		static math::Rectu s_cursorRect;
		static std::vector<float> s_cursorBatchData;
	};

} // gui

} // sh

#endif