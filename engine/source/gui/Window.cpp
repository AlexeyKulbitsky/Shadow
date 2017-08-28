#include "Window.h"

#include "Sprite.h"
#include "Text.h"
#include "Layout.h"
#include "Style.h"
#include "GuiManager.h"
#include "MenuBar.h"
#include "ToolBar.h"

#include "../Device.h"

namespace sh
{
	 
namespace gui
{

	Window::Window(const math::Recti& rect)
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetWindow();
		m_outSprite = ref->m_outSprite;
		m_inSprite = ref->m_inSprite;
		m_barSprite = ref->m_barSprite;
		m_topMargin = 5U;// ref->m_topMargin;
		m_rightMargin = 5U;// ref->m_rightMargin;
		m_bottomMargin = 5U;// ref->m_bottomMargin;
		m_leftMargin = 5U;// ref->m_leftMargin;

		m_batchData.resize(10 * 9);
		m_rect = rect;
		

		UpdatePosition();
		UpdateUV(m_outSprite->GetUVRect().upperLeftCorner, m_outSprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_outSprite->GetColor());

		m_text.reset(new Text(m_barRect));
	}

	///////////////////////////////////////////////////////////////////////////////////////

	Window::Window(const SpritePtr& outSprite, const SpritePtr& inSprite, const SpritePtr& barSprite)
	{
		m_outSprite = outSprite;
		m_inSprite = inSprite;
		m_barSprite = barSprite;

		const auto& outRect = outSprite->GetRect();
		const auto& inRect = inSprite->GetRect();
		const auto& barRect = barSprite->GetRect();

		m_topMargin = barRect.upperLeftCorner.y - outRect.upperLeftCorner.y;
		m_rightMargin = outRect.lowerRightCorner.x - inRect.lowerRightCorner.x;
		m_bottomMargin = outRect.lowerRightCorner.y - inRect.lowerRightCorner.y;
		m_leftMargin = inRect.upperLeftCorner.x - outRect.upperLeftCorner.x;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::SetText(const String& text)
	{
		m_text->SetText(text);
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::SetPosition(s32 x, s32 y)
	{
		Widget::SetPosition(x, y);
		m_text->SetPosition(x, y);

		// Update bar rect for input handling (moving with mouse)
		m_barRect.upperLeftCorner.x = m_rect.upperLeftCorner.x + m_leftMargin;
		m_barRect.upperLeftCorner.y = m_rect.upperLeftCorner.y + m_topMargin;
		m_barRect.lowerRightCorner.x = m_rect.lowerRightCorner.x - m_rightMargin;
		m_barRect.lowerRightCorner.y = m_rect.upperLeftCorner.y + m_topMargin + m_barWidth;

		// Update in rect for updating layout representation
		m_inRect.upperLeftCorner.x = m_rect.upperLeftCorner.x + m_leftMargin;
		m_inRect.upperLeftCorner.y = m_rect.upperLeftCorner.y + m_topMargin + m_barWidth;
		m_inRect.lowerRightCorner.x = m_rect.lowerRightCorner.x - m_rightMargin;
		m_inRect.lowerRightCorner.y = m_rect.lowerRightCorner.y - m_bottomMargin;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::Render(video::Painter* painter)
	{
		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		// Render bar
		video::Painter::Vertex barUL(m_barRect.upperLeftCorner, 
								m_barSprite->GetUVRect().upperLeftCorner, 
								m_barSprite->GetColor());
		video::Painter::Vertex barBR(m_barRect.lowerRightCorner,
								m_barSprite->GetUVRect().lowerRightCorner,
								m_barSprite->GetColor());
		painter->DrawRect(barUL, barBR);

		// Render layout
		video::Painter::Vertex layoutUL(m_inRect.upperLeftCorner, 
								m_inSprite->GetUVRect().upperLeftCorner, 
								m_inSprite->GetColor());
		video::Painter::Vertex layoutBR(m_inRect.lowerRightCorner,
								m_inSprite->GetUVRect().lowerRightCorner,
								m_inSprite->GetColor());
		painter->DrawRect(layoutUL, layoutBR);

		m_text->Render(painter);

		if (m_layout)
		{
			m_layout->Render(painter);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////

	bool Window::ProcessEvent(GUIEvent& ev)
	{
		if (Widget::ProcessEvent(ev))
			return true;
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::UpdateLayout()
	{
		if (m_layout)
		{
			m_layout->Resize(m_inRect);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh