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

		m_rect = rect;
		
		// Update bar rect for input handling (moving with mouse)
		m_barRect.upperLeftCorner.x = m_rect.upperLeftCorner.x;
		m_barRect.upperLeftCorner.y = m_rect.upperLeftCorner.y;
		m_barRect.lowerRightCorner.x = m_rect.lowerRightCorner.x;
		m_barRect.lowerRightCorner.y = m_rect.upperLeftCorner.y + m_barWidth;

		// Update in rect for updating layout representation
		m_inRect.upperLeftCorner.x = m_rect.upperLeftCorner.x;
		m_inRect.upperLeftCorner.y = m_rect.upperLeftCorner.y + m_barWidth;
		m_inRect.lowerRightCorner.x = m_rect.lowerRightCorner.x;
		m_inRect.lowerRightCorner.y = m_rect.lowerRightCorner.y;

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
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::SetText(const String& text)
	{
		m_text->SetText(text);
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::SetRect(const math::Recti& rect)
	{
		m_rect = rect;
		InternalUpdate();
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::SetPosition(s32 x, s32 y)
	{
		auto size = m_rect.GetSize();
		m_rect.Set(x, y, x + size.x, y + size.y);
		InternalUpdate();
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::SetSize(const math::Vector2i& size)
	{
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
		InternalUpdate();
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::SetWidth(s32 width)
	{
		auto size = m_rect.GetSize();
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + width, pos.y + size.y);
		InternalUpdate();
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::SetHeight(s32 height)
	{
		auto size = m_rect.GetSize();
		const auto& pos = m_rect.upperLeftCorner;
		m_rect.Set(pos.x, pos.y, pos.x + size.x, pos.y + height);
		InternalUpdate();
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::Render(video::Painter* painter)
	{
		if (!m_visible)
			return;

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
		if (!m_visible || !m_enabled)
			return false;

		if (Widget::ProcessEvent(ev))
			return true;


		switch (ev.type)
		{
		case EventType::PointerDown:
		{
			if (m_barRect.IsPointInside(ev.x, ev.y) && !m_dragStarted)
			{
				m_startPos.x = ev.x;
				m_startPos.y = ev.y;
				m_dragStarted = true;

				return true;
			}
			else
			{
				m_dragStarted = false;
				if (m_inRect.IsPointInside(ev.x, ev.y))
					return true;
			}
		}
		break;
		case EventType::PointerUp:
		{
			m_dragStarted = false;
			if (m_barRect.IsPointInside(ev.x, ev.y) || m_inRect.IsPointInside(ev.x, ev.y))
			{
				return true;
			}
		}
		break;
		case EventType::PointerMove:
		{
			if (m_dragStarted && m_isMovable)
			{
				sh::Device* device = sh::Device::GetInstance();
				sh::InputManager* inputManager = device->GetInputManager();

				if (inputManager->IsMouseButtonPressed(MouseCode::ButtonLeft))
				{
					math::Vector2i delta = math::Vector2i(ev.x, ev.y) - m_startPos;
					m_startPos.x = ev.x;
					m_startPos.y = ev.y;
					s32 newX = static_cast<s32>(m_rect.upperLeftCorner.x) + delta.x;
					s32 newY = static_cast<s32>(m_rect.upperLeftCorner.y) + delta.y;

					const auto& viewport = device->GetDriver()->GetViewPort();
					if (newX < 0) newX = 0;
					if ((newX + m_rect.GetWidth()) > viewport.z) newX = viewport.z - m_rect.GetWidth();

					u32 topEdge = 0U;
					if (newY < topEdge) newY = topEdge;
					if ((newY + m_rect.GetHeight()) > viewport.w) newY = viewport.w - m_rect.GetHeight();

					SetPosition(newX, newY);
					m_text->SetPosition(newX, newY);

					return true;
				}
			}
		}
		break;
		}


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

	void Window::InternalUpdate()
	{
		m_text->SetPosition(m_rect.upperLeftCorner.x, m_rect.upperLeftCorner.y);

		// Update bar rect for input handling (moving with mouse)
		m_barRect.upperLeftCorner.x = m_rect.upperLeftCorner.x;
		m_barRect.upperLeftCorner.y = m_rect.upperLeftCorner.y;
		m_barRect.lowerRightCorner.x = m_rect.lowerRightCorner.x;
		m_barRect.lowerRightCorner.y = m_rect.upperLeftCorner.y + m_barWidth;

		// Update in rect for updating layout representation
		m_inRect.upperLeftCorner.x = m_rect.upperLeftCorner.x;
		m_inRect.upperLeftCorner.y = m_rect.upperLeftCorner.y + m_barWidth;
		m_inRect.lowerRightCorner.x = m_rect.lowerRightCorner.x;
		m_inRect.lowerRightCorner.y = m_rect.lowerRightCorner.y;

		UpdateLayout();
	}

	///////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh