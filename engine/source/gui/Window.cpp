#include "Window.h"

#include "Sprite.h"
#include "Text.h"
#include "Layout.h"
#include "Style.h"
#include "GuiManager.h"

#include "../Device.h"

namespace sh
{
	 
namespace gui
{

	Window::Window(const math::Rectu& rect)
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetWindow();
		m_outSprite = ref->m_outSprite;
		m_inSprite = ref->m_inSprite;
		m_topMargin = 5U;// ref->m_topMargin;
		m_rightMargin = 5U;// ref->m_rightMargin;
		m_bottomMargin = 5U;// ref->m_bottomMargin;
		m_leftMargin = 5U;// ref->m_leftMargin;

		m_batchData.resize(8 * 8);
		m_rect = rect;

		math::Rectu textRect(m_rect.upperLeftCorner,
			math::Vector2u(m_rect.lowerRightCorner.x, m_rect.upperLeftCorner.y + 15));
		m_text.reset(new Text(textRect));

		UpdatePosition();
		UpdateUV(m_outSprite->GetUVRect().upperLeftCorner, m_outSprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_outSprite->GetColor());
	}

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

		m_topMargin = 25U;
		m_rightMargin = 5U;
		m_bottomMargin = 5U;
		m_leftMargin = 5U;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	Window::Window(const SpritePtr& outSprite, const SpritePtr& inSprite)
	{
		m_outSprite = outSprite;
		m_inSprite = inSprite;

		const auto& outRect = outSprite->GetRect();
		const auto& inRect = inSprite->GetRect();

		m_topMargin = inRect.upperLeftCorner.y - outRect.upperLeftCorner.y;
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

	void Window::GetGeometry(GuiBatchData& data)
	{
		//Widget::GetGeometry(data);

		data.vertices.insert(data.vertices.end(), m_batchData.begin(), m_batchData.end());

		
		const u32 count = data.verticesCount;
		
		data.indices.push_back(count);
		data.indices.push_back(count + 1);
		data.indices.push_back(count + 5);
		data.indices.push_back(count);
		data.indices.push_back(count + 5);
		data.indices.push_back(count + 4);
		
		
		
		data.indices.push_back(count + 1);
		data.indices.push_back(count + 6);
		data.indices.push_back(count + 5);
		data.indices.push_back(count + 1);
		data.indices.push_back(count + 2);
		data.indices.push_back(count + 6);
		

		
		data.indices.push_back(count + 2);
		data.indices.push_back(count + 3);
		data.indices.push_back(count + 7);
		data.indices.push_back(count + 2);
		data.indices.push_back(count + 7);
		data.indices.push_back(count + 6);
		

		
		data.indices.push_back(count + 3);
		data.indices.push_back(count);
		data.indices.push_back(count + 4);
		data.indices.push_back(count + 3);
		data.indices.push_back(count + 4);
		data.indices.push_back(count + 7);

		
		data.indices.push_back(count + 5);
		data.indices.push_back(count + 7);
		data.indices.push_back(count + 4);
		data.indices.push_back(count + 5);
		data.indices.push_back(count + 6);
		data.indices.push_back(count + 7);
		
		data.verticesCount += 8;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::GetTextGeometry(GuiBatchData& data)
	{
		m_text->GetTextGeometry(data);
	}

	///////////////////////////////////////////////////////////////////////////////////////

	bool Window::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		if (Widget::ProcessInput(x, y, type))
			return true;

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

	void Window::UpdatePosition()
	{
		math::Vector4f leftUp((float)m_rect.upperLeftCorner.x, (float)m_rect.upperLeftCorner.y, 0.0f, 1.0f);
		math::Vector4f rightDown((float)m_rect.lowerRightCorner.x, (float)m_rect.lowerRightCorner.y, 0.0f, 1.0f);

		m_batchData[0] = leftUp.x; m_batchData[1] = leftUp.y; m_batchData[2] = 0.0f;
		m_batchData[8] = leftUp.x; m_batchData[9] = rightDown.y; m_batchData[10] = 0.0f;
		m_batchData[16] = rightDown.x; m_batchData[17] = rightDown.y; m_batchData[18] = 0.0f;
		m_batchData[24] = rightDown.x; m_batchData[25] = leftUp.y; m_batchData[26] = 0.0f;

		m_batchData[32] = leftUp.x + m_leftMargin; m_batchData[33] = leftUp.y + m_topMargin; m_batchData[34] = 0.0f;
		m_batchData[40] = leftUp.x + m_leftMargin; m_batchData[41] = rightDown.y - m_bottomMargin; m_batchData[42] = 0.0f;
		m_batchData[48] = rightDown.x - m_rightMargin; m_batchData[49] = rightDown.y - m_bottomMargin; m_batchData[50] = 0.0f;
		m_batchData[56] = rightDown.x - m_rightMargin; m_batchData[57] = leftUp.y + m_topMargin; m_batchData[58] = 0.0f;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::UpdateUV(const math::Vector2f& uvLeftUp, const math::Vector2f& uvRightDown)
	{
		m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y;
		m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y;
		m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y;
		m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y;

		const auto& inRect = m_inSprite->GetUVRect();
		m_batchData[35] = inRect.upperLeftCorner.x; m_batchData[36] = inRect.upperLeftCorner.y;
		m_batchData[43] = inRect.upperLeftCorner.x; m_batchData[44] = inRect.lowerRightCorner.y;
		m_batchData[51] = inRect.lowerRightCorner.x; m_batchData[52] = inRect.lowerRightCorner.y;
		m_batchData[59] = inRect.lowerRightCorner.x; m_batchData[60] = inRect.upperLeftCorner.y;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::UpdateColor(const math::Vector3f& color)
	{
		m_batchData[5] = color.x; m_batchData[6] = color.y; m_batchData[7] = color.z;
		m_batchData[13] = color.x; m_batchData[14] = color.y; m_batchData[15] = color.z;
		m_batchData[21] = color.x; m_batchData[22] = color.y; m_batchData[23] = color.z;
		m_batchData[29] = color.x; m_batchData[30] = color.y;  m_batchData[31] = color.z;

		m_batchData[37] = color.x; m_batchData[38] = color.y; m_batchData[39] = color.z;
		m_batchData[45] = color.x; m_batchData[46] = color.y; m_batchData[47] = color.z;
		m_batchData[53] = color.x; m_batchData[54] = color.y; m_batchData[55] = color.z;
		m_batchData[61] = color.x; m_batchData[62] = color.y;  m_batchData[63] = color.z;
	}

	///////////////////////////////////////////////////////////////////////////////////////


} // gui

} // sh