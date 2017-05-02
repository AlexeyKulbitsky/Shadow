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