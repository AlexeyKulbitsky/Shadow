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
#include "../serialization/ObjectFactory.h"

namespace sh
{

namespace gui
{
	math::Rect LineEdit::s_cursorRect;
	size_t LineEdit::s_cursorPos = 0U;

	LineEdit::LineEdit()
	{
		const auto& ref = GuiManager::GetInstance()->GetStyle()->GetLineEdit();

		m_sprites[State::Default] = ref->m_sprites[State::Default];
		m_sprites[State::Edit] = ref->m_sprites[State::Edit];
        m_name = "LineEdit";
	}

	LineEdit::LineEdit(const SpritePtr& defaultSprite,
		const SpritePtr& editSprite)
	{
		m_sprites[State::Default] = defaultSprite;
		m_sprites[State::Edit] = editSprite;
        m_name = "LineEdit";
	}

	LineEdit::LineEdit(const math::Rect& rect, 
						const SpritePtr& defaultSprite, 
						 const SpritePtr& editSprite)
						 :Text(rect)
	{
		m_sprites[State::Default] = defaultSprite;
		m_sprites[State::Edit] = editSprite;
        m_name = "LineEdit";
	}
    
    void LineEdit::RegisterObject()
    {
        ObjectFactory::GetInstance()->RegisterFactory<LineEdit>("UI");
        ObjectFactory::GetInstance()->RegisterParentProperties<LineEdit, Text>();
    }
    
	void LineEdit::SetState(State state)
	{
		switch (state)
		{
			case State::Default:
			{
				GuiManager::GetInstance()->SetFocusWidget(nullptr);
			}
				break;
			case State::Edit:
			{
				s_cursorPos = m_text.size();
				GuiManager::GetInstance()->SetFocusWidget(shared_from_this());
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
										m_sprites[m_state]->GetUpperLeftUV(),
										m_sprites[m_state]->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
			m_sprites[m_state]->GetLowerRightUV(),
			m_sprites[m_state]->GetColor());
		painter->DrawRect(upperLeft, downRight);

		// Render text
		Text::Render(painter);
		
		if (IsInFocus())
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
				math::Vector2(0.0f),
				math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			video::Painter::Vertex downRight(s_cursorRect.lowerRightCorner,
				math::Vector2(0.0f),
				math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			painter->DrawRect(upperLeft, downRight);
		}
	}

	void LineEdit::RenderBackground(video::Painter* painter)
	{
		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner,
			m_sprites[m_state]->GetUpperLeftUV(),
			m_sprites[m_state]->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
			m_sprites[m_state]->GetLowerRightUV(),
			m_sprites[m_state]->GetColor());
		painter->DrawRect(upperLeft, downRight);
	}

	void LineEdit::RenderText(video::Painter* painter)
	{
		Text::Render(painter);

		if (IsInFocus())
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
				math::Vector2(0.0f),
				math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			video::Painter::Vertex downRight(s_cursorRect.lowerRightCorner,
				math::Vector2(0.0f),
				math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			painter->DrawRect(upperLeft, downRight);
		}
	}

	bool LineEdit::ProcessEvent(GUIEvent& ev)
	{
		bool inside = m_rect.IsPointInside(ev.x, ev.y);
		switch (ev.type)
		{
			case EventType::PointerDown:
			{
				if (inside)
				{
					if (IsInFocus())
					{
						m_state = State::Default;
						UpdateIfDirty();
						GuiManager::GetInstance()->SetFocusWidget(nullptr);
					}
					else
					{
						m_state = State::Edit;
						s_cursorPos = m_text.size();
						GuiManager::GetInstance()->SetFocusWidget(shared_from_this());
					}

					return true;
				}
				else
				{
					if (IsInFocus())
					{
						GuiManager::GetInstance()->SetFocusWidget(nullptr);

						m_state = State::Default;
						UpdateIfDirty();

						return true;
					}
				}
			}
				break;
			case EventType::PointerUp:
			{
				if (IsInFocus())
				{
					return true;
				}
			}
				break;
			case EventType::PointerMove:
				break;
			case EventType::KeyDown:
			{
				if (IsInFocus())
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
						GuiManager::GetInstance()->SetFocusWidget(nullptr);
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
			}
				break;
			default:
				break;
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
