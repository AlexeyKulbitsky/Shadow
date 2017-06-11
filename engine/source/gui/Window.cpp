#include "Window.h"

#include "Sprite.h"
#include "Text.h"

#include "../Device.h"

namespace sh
{
	 
namespace gui
{

	Window::Window(const math::Rectu& rect, const SpritePtr& sprite)
	{
		m_batchData.resize(4 * 8);
		m_rect = rect;
		m_sprite = sprite;

		math::Rectu textRect(m_rect.upperLeftCorner,
			math::Vector2u(m_rect.lowerRightCorner.x, m_rect.upperLeftCorner.y + 15));
		m_text.reset(new Text(textRect));

		UpdatePosition();
		UpdateUV(m_sprite->GetUVRect().upperLeftCorner, m_sprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_sprite->GetColor());
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::SetText(const String& text)
	{
		m_text->SetText(text);
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::GetGeometry(GuiBatchData& data)
	{
		GuiElement::GetGeometry(data);
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::GetTextGeometry(GuiBatchData& data)
	{
		m_text->GetTextGeometry(data);
	}

	///////////////////////////////////////////////////////////////////////////////////////

	bool Window::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		switch (type)
		{
		case MouseEventType::ButtonPressed:
		{
			if (m_rect.IsPointInside(x, y) && !m_dragStarted)
			{
				m_startPos.x = static_cast<s32>(x);
				m_startPos.y = static_cast<s32>(y);
				m_dragStarted = true;
				
				return true;
			}
			else
			{
				m_dragStarted = false;
			}
		}
		break;
		case MouseEventType::ButtonReleased:
		{
			m_dragStarted = false;
			
			return false;
		}
		break;
		case MouseEventType::Moved:
		{
			if (m_dragStarted)
			{
				sh::Device* device = sh::Device::GetInstance();
				sh::InputManager* inputManager = device->GetInputManager();

				if (inputManager->IsMouseButtonPressed(MouseCode::ButtonLeft))
				{
					math::Vector2i delta = math::Vector2i(x, y) - m_startPos;
					m_startPos.x = x;
					m_startPos.y = y;
					s32 newX = static_cast<s32>(m_rect.upperLeftCorner.x) + delta.x;
					s32 newY = static_cast<s32>(m_rect.upperLeftCorner.y) + delta.y;

					SetPosition(static_cast<u32>(newX), static_cast<u32>(newY));
					m_text->SetPosition(static_cast<u32>(newX), static_cast<u32>(newY));
					
					return true;
				}
			}
		}
		break;
		}

		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh