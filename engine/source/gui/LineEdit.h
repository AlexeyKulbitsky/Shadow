#ifndef SHADOW_LINE_EDIT_INCLUDE
#define SHADOW_LINE_EDIT_INCLUDE

#include "GuiElement.h"
#include "Text.h"

namespace sh
{

namespace gui
{

	class LineEdit : public Text//public GuiElement
	{
	public:
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

	protected:
		SpritePtr m_defaultSprite;
		SpritePtr m_editSprite;

		//String m_text = "";
		//std::vector<float> m_textBatchData;

		bool m_dirty = false;
		bool m_inFocus = true;
	};

} // gui

} // sh

#endif