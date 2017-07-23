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
		m_barSprite = ref->m_barSprite;
		m_topMargin = 5U;// ref->m_topMargin;
		m_rightMargin = 5U;// ref->m_rightMargin;
		m_bottomMargin = 5U;// ref->m_bottomMargin;
		m_leftMargin = 5U;// ref->m_leftMargin;

		m_batchData.resize(10 * 8);
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

	void Window::GetGeometry(GuiBatchData& data)
	{
		//Widget::GetGeometry(data);

		data.vertices.insert(data.vertices.end(), m_batchData.begin(), m_batchData.end());
		
		const u32 count = data.verticesCount;
		
		data.indices.push_back(count);
		data.indices.push_back(count + 1);
		data.indices.push_back(count + 6);
		data.indices.push_back(count);
		data.indices.push_back(count + 6);
		data.indices.push_back(count + 4);
		
		data.indices.push_back(count + 1);
		data.indices.push_back(count + 7);
		data.indices.push_back(count + 6);
		data.indices.push_back(count + 1);
		data.indices.push_back(count + 2);
		data.indices.push_back(count + 7);
		
		data.indices.push_back(count + 2);
		data.indices.push_back(count + 3);
		data.indices.push_back(count + 9);
		data.indices.push_back(count + 2);
		data.indices.push_back(count + 9);
		data.indices.push_back(count + 7);
		
		data.indices.push_back(count + 3);
		data.indices.push_back(count);
		data.indices.push_back(count + 4);
		data.indices.push_back(count + 3);
		data.indices.push_back(count + 4);
		data.indices.push_back(count + 9);
		
		data.indices.push_back(count + 5);
		data.indices.push_back(count + 9);
		data.indices.push_back(count + 4);
		data.indices.push_back(count + 5);
		data.indices.push_back(count + 8);
		data.indices.push_back(count + 9);

		data.indices.push_back(count + 6);
		data.indices.push_back(count + 8);
		data.indices.push_back(count + 5);
		data.indices.push_back(count + 6);
		data.indices.push_back(count + 7);
		data.indices.push_back(count + 8);
		
		data.verticesCount += 10;

		if (m_layout)
		{
			m_layout->GetGeometry(data);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::GetTextGeometry(GuiBatchData& data)
	{
		m_text->GetTextGeometry(data);

		if (m_layout)
		{
			m_layout->GetTextGeometry(data);
		}
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
			if (m_barRect.IsPointInside(x, y) && !m_dragStarted)
			{
				m_startPos.x = static_cast<s32>(x);
				m_startPos.y = static_cast<s32>(y);
				m_dragStarted = true;
				
				return true;
			}
			else
			{
				m_dragStarted = false;
				if (m_inRect.IsPointInside(x, y))
					return true;
			}
		}
		break;
		case MouseEventType::ButtonReleased:
		{
			m_dragStarted = false;
			if (m_barRect.IsPointInside(x, y) || m_inRect.IsPointInside(x, y))
			{
				return true;
			}
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

					const auto& viewport = device->GetDriver()->GetViewPort();
					if (newX < 0) newX = 0;
					if ((newX + m_rect.GetWidth()) > viewport.z) newX = viewport.z - m_rect.GetWidth();
					if (newY < 0) newY = 0;
					if ((newY + m_rect.GetHeight()) > viewport.w) newY = viewport.w - m_rect.GetHeight();

					SetPosition(static_cast<u32>(newX), static_cast<u32>(newY));
					m_text->SetPosition(static_cast<u32>(newX) + m_leftMargin, 
						static_cast<u32>(newY) + m_topMargin);
					
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
		m_batchData[40] = leftUp.x + m_leftMargin; m_batchData[41] = leftUp.y + m_topMargin + m_barWidth; m_batchData[42] = 0.0f;
		m_batchData[48] = leftUp.x + m_leftMargin; m_batchData[49] = rightDown.y - m_bottomMargin; m_batchData[50] = 0.0f;
		m_batchData[56] = rightDown.x - m_rightMargin; m_batchData[57] = rightDown.y - m_bottomMargin; m_batchData[58] = 0.0f;
		m_batchData[64] = rightDown.x - m_rightMargin; m_batchData[65] = leftUp.y + m_topMargin + m_barWidth; m_batchData[66] = 0.0f;
		m_batchData[72] = rightDown.x - m_rightMargin; m_batchData[73] = leftUp.y + m_topMargin; m_batchData[74] = 0.0f;

		// Update bar rect for input handling (moving with mouse)
		m_barRect.upperLeftCorner.x = m_rect.upperLeftCorner.x + m_leftMargin;
		m_barRect.upperLeftCorner.y = m_rect.upperLeftCorner.y + m_topMargin;
		m_barRect.lowerRightCorner.x = m_rect.lowerRightCorner.x - m_rightMargin;
		m_barRect.lowerRightCorner.y = m_rect.upperLeftCorner.y + m_topMargin + m_barWidth;

		// Update in rect for updating layout representation
		m_inRect.upperLeftCorner.x = m_rect.upperLeftCorner.x + m_leftMargin;
		m_inRect.upperLeftCorner.y = m_rect.upperLeftCorner.y + m_topMargin + m_barWidth + m_topMargin;
		m_inRect.lowerRightCorner.x = m_rect.lowerRightCorner.x - m_rightMargin;
		m_inRect.lowerRightCorner.y = m_rect.lowerRightCorner.y - m_bottomMargin;
	}

	///////////////////////////////////////////////////////////////////////////////////////

	void Window::UpdateUV(const math::Vector2f& uvLeftUp, const math::Vector2f& uvRightDown)
	{
		m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y;
		m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y;
		m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y;
		m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y;

		const auto& inRect = m_inSprite->GetUVRect();
		const auto& barRect = m_barSprite->GetUVRect();

		m_batchData[35] = barRect.upperLeftCorner.x; m_batchData[36] = barRect.upperLeftCorner.y;
		m_batchData[43] = barRect.upperLeftCorner.x; m_batchData[44] = barRect.lowerRightCorner.y;
		m_batchData[51] = inRect.upperLeftCorner.x; m_batchData[52] = inRect.lowerRightCorner.y;
		m_batchData[59] = inRect.lowerRightCorner.x; m_batchData[60] = inRect.lowerRightCorner.y;
		m_batchData[67] = barRect.lowerRightCorner.x; m_batchData[68] = barRect.lowerRightCorner.y;
		m_batchData[75] = barRect.lowerRightCorner.x; m_batchData[76] = barRect.upperLeftCorner.y;
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
		m_batchData[69] = color.x; m_batchData[70] = color.y;  m_batchData[71] = color.z;
		m_batchData[77] = color.x; m_batchData[78] = color.y;  m_batchData[79] = color.z;
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