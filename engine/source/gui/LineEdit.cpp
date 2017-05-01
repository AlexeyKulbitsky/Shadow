#include "LineEdit.h"

#include "GuiManager.h"
#include "Sprite.h"

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
	LineEdit::LineEdit(const math::Rectu& rect, 
						const SpritePtr& defaultSprite, 
						 const SpritePtr& editSprite)
						 :Text(rect)
	{
		m_batchData.resize(4 * 8);
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
		data.vertices.insert(data.vertices.end(), m_batchData.begin(), m_batchData.end());

		data.indices.push_back(data.verticesCount);
		data.indices.push_back(data.verticesCount + 1);
		data.indices.push_back(data.verticesCount + 2);

		data.indices.push_back(data.verticesCount);
		data.indices.push_back(data.verticesCount + 2);
		data.indices.push_back(data.verticesCount + 3);
		data.verticesCount += 4;
	}

	///*
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

			float x1 = 2.0f * (float)(xOrigin + desc.x_off) / viewPort.z - 1.0f;
			float y1 = 2.0f * (viewPort.w - (float)(yOrigin - desc.y_off)) / viewPort.w - 1.0f;
			float x2 = 2.0f * (float)(xOrigin + desc.x_off + desc.width) / viewPort.z - 1.0f;
			float y2 = 2.0f * (viewPort.w - (float)(yOrigin - desc.y_off + desc.height)) / viewPort.w - 1.0f;

			float u1 = (float)desc.x0 / (float)width;
			float v1 = (float)desc.y0 / (float)height;
			float u2 = (float)desc.x1 / (float)width;
			float v2 = (float)desc.y1 / (float)height;

			data.vertices.insert(data.vertices.end(), {x1, y1, 0.0f, u1, v1});
			data.vertices.insert(data.vertices.end(), {x1, y2, 0.0f, u1, v2});
			data.vertices.insert(data.vertices.end(), {x2, y2, 0.0f, u2, v2});
			data.vertices.insert(data.vertices.end(), {x2, y1, 0.0f, u2, v1});

			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 1);
			data.indices.push_back(data.verticesCount + 2);

			data.indices.push_back(data.verticesCount);
			data.indices.push_back(data.verticesCount + 2);
			data.indices.push_back(data.verticesCount + 3);
			data.verticesCount += 4;

			xOrigin += desc.advance;
		}
	}
	//*/

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

	
} // gui

} // sh