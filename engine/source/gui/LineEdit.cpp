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

		m_sprites[State::Default] = ref->m_sprites[State::Default];
		m_sprites[State::Edit] = ref->m_sprites[State::Edit];
	}

	LineEdit::LineEdit(const SpritePtr& defaultSprite,
		const SpritePtr& editSprite)
	{
		m_sprites[State::Default] = defaultSprite;
		m_sprites[State::Edit] = editSprite;
	}

	LineEdit::LineEdit(const math::Recti& rect, 
						const SpritePtr& defaultSprite, 
						 const SpritePtr& editSprite)
						 :Text(rect)
	{
		m_sprites[State::Default] = defaultSprite;
		m_sprites[State::Edit] = editSprite;
	}

	/*
	void LineEdit::GetTextGeometry(GuiBatchData& data)
	{
		if (m_inFocus)
		{
			s_cursorRect.Set(static_cast<u32>(xOrigin), m_rect.upperLeftCorner.y,
				static_cast<u32>(xOrigin) + 3U, m_rect.lowerRightCorner.y);
		}
	}
	*/

	void LineEdit::Render(video::Painter* painter)
	{
		// Render background
		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner,
										m_sprites[m_state]->GetUVRect().upperLeftCorner,
										m_sprites[m_state]->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
			m_sprites[m_state]->GetUVRect().lowerRightCorner,
			m_sprites[m_state]->GetColor());
		painter->DrawRect(upperLeft, downRight);

		// Render text
		Text::Render(painter);
	}

	bool LineEdit::ProcessEvent(GUIEvent& ev)
	{
		if (ev.type == EventType::PointerDown)
		{
			bool inside = m_rect.IsPointInside(ev.x, ev.y);
			if (inside)
			{
				if (m_inFocus)
				{
					m_state = State::Default;
					UpdateIfDirty();
				}
				else
				{
					m_state = State::Edit;
				}
				m_inFocus = !m_inFocus;

				return true;
			}
			else
			{
				if (m_inFocus)
				{
					m_inFocus = false;

					m_state = State::Default;
					UpdateIfDirty();

					return true;
				}
			}
		}
		else if (m_inFocus && ev.type == EventType::KeyDown)
		{
			if (ev.keyCode == (int)KeyCode::KEY_BACK)
			{
				if (m_text.size() >= 1)
				{
					m_text.pop_back();
					m_dirty = true;
				}
			}
			else if (ev.keyCode == (int)KeyCode::KEY_RETURN)
			{
				m_inFocus = false;
				m_state = State::Default;
				UpdateIfDirty();
			}
			else
			{
				m_text += (char)ev.keyCode;
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

	/*
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
	*/
	
} // gui

} // sh