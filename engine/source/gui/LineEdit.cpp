#include "LineEdit.h"

#include "GuiManager.h"
#include "Sprite.h"
#include "Style.h"

#include "../scene/SceneManager.h"
#include "../scene/Camera.h"
#include "../video/VertexDeclaration.h"
#include "../video/Driver.h"

#include "../font/Font.h"
#include "../Device.h"

namespace sh
{

namespace gui
{
	math::Rectu LineEdit::s_cursorRect;
	std::vector<float> LineEdit::s_cursorBatchData;

	LineEdit::LineEdit()
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetLineEdit();
		m_batchData.resize(4 * 9);
		m_defaultSprite = ref->m_defaultSprite;
		m_editSprite = ref->m_editSprite;

		UpdatePosition();
		UpdateUV(m_defaultSprite->GetUVRect().upperLeftCorner, m_defaultSprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_defaultSprite->GetColor());
	}

	LineEdit::LineEdit(const SpritePtr& defaultSprite,
		const SpritePtr& editSprite)
	{
		m_batchData.resize(4 * 9);
		m_defaultSprite = defaultSprite;
		m_editSprite = editSprite;
	}

	LineEdit::LineEdit(const math::Rectu& rect, 
						const SpritePtr& defaultSprite, 
						 const SpritePtr& editSprite)
						 :Text(rect)
	{
		m_batchData.resize(4 * 9);
		//m_rect = rect;
		m_defaultSprite = defaultSprite;
		m_editSprite = editSprite;

		UpdatePosition();
		UpdateUV(m_defaultSprite->GetUVRect().upperLeftCorner, 
				 m_defaultSprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_defaultSprite->GetColor());
	}

	void LineEdit::GetGeometry(GuiBatchData& data)
	{
		Widget::GetGeometry(data);

		if (m_inFocus)
		{
			//s_cursorRect.Set(m_rect.upperLeftCorner, math::Vector2u(m_rect.upperLeftCorner.x + 3U, m_rect.lowerRightCorner.y));
			UpdateCursorGeometry();

			data.vertices.insert(data.vertices.end(), s_cursorBatchData.begin(), s_cursorBatchData.end());

			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 1);
			data.indices.push_back(data.verticesCount + 2);

			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 2);
			data.indices.push_back(data.verticesCount + 3);
			data.verticesCount += 4;
		}
	}

	void LineEdit::GetTextGeometry(GuiBatchData& data)
	{
		const auto& viewPort = sh::Device::GetInstance()->GetDriver()->GetViewPort();

		const auto& font = GuiManager::GetInstance()->GetFont();
		s32 width = font->GetTextureAtlas()->GetDescription().width;
		s32 height = font->GetTextureAtlas()->GetDescription().height;

		s32 xOrigin = m_rect.upperLeftCorner.x + 5;
		s32 yOrigin = m_rect.lowerRightCorner.y - m_rect.GetHeight() / 4;

		for (const auto c : m_text)
		{
			const auto& desc = font->GetGlyphDescription(static_cast<u32>(c));

			float x1 = static_cast<float>(xOrigin + desc.x_off);
			float y1 = static_cast<float>(yOrigin - desc.y_off);
			float x2 = static_cast<float>(xOrigin + desc.x_off + desc.width);
			float y2 = static_cast<float>(yOrigin - desc.y_off + desc.height);

			float u1 = (float)desc.x0 / (float)width;
			float v1 = (float)desc.y0 / (float)height;
			float u2 = (float)desc.x1 / (float)width;
			float v2 = (float)desc.y1 / (float)height;

			data.vertices.insert(data.vertices.end(), { x1, y1, 0.0f, u1, v1, 0.0f, 0.0f, 0.0f, 1.0f });
			data.vertices.insert(data.vertices.end(), { x1, y2, 0.0f, u1, v2, 0.0f, 0.0f, 0.0f, 1.0f });
			data.vertices.insert(data.vertices.end(), { x2, y2, 0.0f, u2, v2, 0.0f, 0.0f, 0.0f, 1.0f });
			data.vertices.insert(data.vertices.end(), { x2, y1, 0.0f, u2, v1, 0.0f, 0.0f, 0.0f, 1.0f });

			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 1);
			data.indices.push_back(data.verticesCount + 2);

			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 2);
			data.indices.push_back(data.verticesCount + 3);
			data.verticesCount += 4;

			xOrigin += desc.advance;
		}

		if (m_inFocus)
		{
			s_cursorRect.Set(static_cast<u32>(xOrigin), m_rect.upperLeftCorner.y,
				static_cast<u32>(xOrigin) + 3U, m_rect.lowerRightCorner.y);
		}
	}

	bool LineEdit::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		if (type == MouseEventType::ButtonPressed)
		{
			bool inside = m_rect.IsPointInside(x, y);
			if (inside)
			{
				if (m_inFocus)
				{
					UpdateUV(m_defaultSprite->GetUVRect().upperLeftCorner, 
						m_defaultSprite->GetUVRect().lowerRightCorner);
					UpdateColor(m_defaultSprite->GetColor());

					UpdateIfDirty();
				}
				else
				{
					UpdateUV(m_editSprite->GetUVRect().upperLeftCorner, 
						 m_editSprite->GetUVRect().lowerRightCorner);
					UpdateColor(m_editSprite->GetColor());
				}
				m_inFocus = !m_inFocus;

				return true;
			}
			else
			{
				if (m_inFocus)
				{
					UpdateUV(m_defaultSprite->GetUVRect().upperLeftCorner, 
							m_defaultSprite->GetUVRect().lowerRightCorner);
					UpdateColor(m_defaultSprite->GetColor());
					m_inFocus = false;

					UpdateIfDirty();

					return true;
				}
			}
		}
		
		return false;
	}

	bool LineEdit::ProcessKeyboardInput(KeyboardEventType type, KeyCode code)
	{
		if (m_inFocus && type == KeyboardEventType::KeyPressed)
		{
			if (code == KeyCode::KEY_BACK)
			{
				if (m_text.size() >= 1)
				{
					m_text.pop_back();
					m_dirty = true;
				}
			}
			else if (code == KeyCode::KEY_RETURN)
			{
				UpdateUV(m_defaultSprite->GetUVRect().upperLeftCorner, 
							m_defaultSprite->GetUVRect().lowerRightCorner);
				UpdateColor(m_defaultSprite->GetColor());
				m_inFocus = false;

				UpdateIfDirty();
			}
			else
			{
				m_text += (char)code;
				m_dirty = true;
			}
				
			return true;
		}
		return false;
	}

	
	void LineEdit::UpdateIfDirty()
	{
		if (m_dirty)
		{
			OnTextChanged(m_text);
			m_dirty = false;
		}
	}

	void LineEdit::UpdateCursorGeometry()
	{
		if (s_cursorBatchData.size() == 0U)
			s_cursorBatchData.resize(4 * 9);

		// Updated positions
		const auto& viewPort = sh::Device::GetInstance()->GetDriver()->GetViewPort();
		math::Vector4f leftUp((float)s_cursorRect.upperLeftCorner.x, (float)s_cursorRect.upperLeftCorner.y, 0.0f, 1.0f);

		math::Vector4f rightDown((float)s_cursorRect.lowerRightCorner.x, (float)s_cursorRect.lowerRightCorner.y, 0.0f, 1.0f);

		s_cursorBatchData[0] = leftUp.x; s_cursorBatchData[1] = leftUp.y; s_cursorBatchData[2] = 0.0f;
		s_cursorBatchData[9] = leftUp.x; s_cursorBatchData[10] = rightDown.y; s_cursorBatchData[11] = 0.0f;
		s_cursorBatchData[18] = rightDown.x; s_cursorBatchData[19] = rightDown.y; s_cursorBatchData[20] = 0.0f;
		s_cursorBatchData[27] = rightDown.x; s_cursorBatchData[28] = leftUp.y; s_cursorBatchData[29] = 0.0f;

		// Update UVs
		s_cursorBatchData[3] = 0.0f; s_cursorBatchData[4] = 0.0f;
		s_cursorBatchData[12] = 0.0f; s_cursorBatchData[13] = 0.0f;
		s_cursorBatchData[21] = 0.0f; s_cursorBatchData[22] = 0.0f;
		s_cursorBatchData[30] = 0.0f; s_cursorBatchData[31] = 0.0f;

		// Update colors
		math::Vector3f color(0.0f);
		s_cursorBatchData[5] = color.x; s_cursorBatchData[6] = color.y; s_cursorBatchData[7] = color.z; s_cursorBatchData[8] = 1.0f;
		s_cursorBatchData[14] = color.x; s_cursorBatchData[15] = color.y; s_cursorBatchData[16] = color.z; s_cursorBatchData[17] = 1.0f;
		s_cursorBatchData[23] = color.x; s_cursorBatchData[24] = color.y; s_cursorBatchData[25] = color.z; s_cursorBatchData[26] = 1.0f;
		s_cursorBatchData[32] = color.x; s_cursorBatchData[33] = color.y;  s_cursorBatchData[34] = color.z; s_cursorBatchData[35] = 1.0f;
	}

	
} // gui

} // sh