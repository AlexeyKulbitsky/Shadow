#ifndef SHADOW_LINE_EDIT_INCLUDE
#define SHADOW_LINE_EDIT_INCLUDE

#include "GuiElement.h"

namespace sh
{

namespace gui
{

	class LineEdit : public GuiElement
	{
	public:
		LineEdit(const math::Rectu& rect, 
			   const SpritePtr& defaultSprite, 
			   const SpritePtr& pressedSprite,
			   const SpritePtr& hoveredSprite);

		virtual void GetGeometry(GuiBatchData& data) override;
		virtual void GetText(GuiBatchData& data) override;
		virtual bool ProcessInput(u32 x, u32 y, MouseEventType type) override;
		virtual bool ProcessKeyboardInput(KeyboardEventType type, KeyCode code) override;

	private:
		void UpdatePosition();
		void UpdateUV(const math::Vector2f& leftUp, const math::Vector2f& rightDown);
		void UpdateColor(const math::Vector3f& color);

	private:
		math::Rectu m_rect;

		SpritePtr m_pressedSprite;
		SpritePtr m_releasedSprite;
		SpritePtr m_hoveredSprite;

		String m_text = "LineEdit";
		std::vector<float> m_textBatchData;

		bool m_inFocus = true;
	};

} // gui

} // sh

#endif