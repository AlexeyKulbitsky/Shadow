#ifndef SHADOW_LINE_EDIT_INCLUDE
#define SHADOW_LINE_EDIT_INCLUDE

#include "GuiElement.h"
#include "Text.h"

namespace sh
{

namespace gui
{

	class LineEdit : public Text
	{
	public:
		LineEdit();
		LineEdit(const SpritePtr& defaultSprite,
				const SpritePtr& editSprite);

		LineEdit(const math::Rectu& rect, 
			   const SpritePtr& defaultSprite, 
			   const SpritePtr& editSprite);

		virtual void GetGeometry(GuiBatchData& data) override;
		virtual void GetTextGeometry(GuiBatchData& data) override;

		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;
		virtual bool ProcessKeyboardInput(KeyboardEventType type, KeyCode code) override;

		Event<void, const String&> OnTextChanged;

	protected:
		virtual void UpdateIfDirty();
		static void UpdateCursorGeometry();

	protected:
		SpritePtr m_defaultSprite;
		SpritePtr m_editSprite;

		bool m_dirty = false;
		bool m_inFocus = false;

		static math::Rectu s_cursorRect;
		static std::vector<float> s_cursorBatchData;
	};

} // gui

} // sh

#endif