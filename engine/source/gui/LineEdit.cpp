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
	math::Recti LineEdit::s_cursorRect;
	size_t LineEdit::s_cursorPos = 0U;

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

	void LineEdit::SetState(State state)
	{
		switch (state)
		{
			case State::Default:
			{
				m_inFocus = false;
			}
				break;
			case State::Edit:
			{
				s_cursorPos = m_text.size();
				m_inFocus = true;
			}
				break;
			default:
				break;
		}
		m_state = state;
	}

	void LineEdit::Render(video::Painter* painter)
	{
		if (!m_visible)
			return;

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

		if (m_inFocus)
		{
			s32 xPos = 0;
			if (s_cursorPos == m_text.size())
			{
				if (m_text.size() == 0U)
				{
					xPos = m_rect.upperLeftCorner.x + 5;
				}
				else
				{
					const auto& font = GuiManager::GetInstance()->GetFont();
					const auto& desc = font->GetGlyphDescription(static_cast<u32>(m_text.back()));
					xPos = m_glyphOffsets.back() + desc.advance;
				}
			}
			else
			{
				xPos = m_glyphOffsets[s_cursorPos];
			}

			s_cursorRect.Set(xPos, m_rect.upperLeftCorner.y,
				xPos + 3U, m_rect.lowerRightCorner.y);

			video::Painter::Vertex upperLeft(s_cursorRect.upperLeftCorner,
				math::Vector2f(0.0f),
				math::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
			video::Painter::Vertex downRight(s_cursorRect.lowerRightCorner,
				math::Vector2f(0.0f),
				math::Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
			painter->DrawRect(upperLeft, downRight);
		}
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
					s_cursorPos = m_text.size();
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
			KeyCode keyCode = static_cast<KeyCode>(ev.keyCode);


			switch (keyCode)
			{
			case KeyCode::KEY_BACK:
			{
				if (m_text.size() >= 1)
				{
					m_text.erase(m_text.begin() + (s_cursorPos - 1));
					s_cursorPos--;
					m_dirty = true;
				}
			}
			break;
			case KeyCode::KEY_RETURN:
			{
				m_inFocus = false;
				m_state = State::Default;
				UpdateIfDirty();
				OnEditingFinished(m_text);
			}
				break;
			case KeyCode::KEY_LEFT:
			{
				if (s_cursorPos != 0U)
					s_cursorPos--;
			}
			break;
			case KeyCode::KEY_RIGHT:
			{
				if (s_cursorPos < m_text.size())
					s_cursorPos++;
			}
			break;
			default:
			{
				m_text.insert(m_text.begin() + s_cursorPos++, (char)ev.keyCode);
				m_dirty = true;
			}
				break;
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