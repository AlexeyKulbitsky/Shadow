#include "FloatLineEdit.h"

#include "Sprite.h"

namespace sh
{

namespace gui
{
	FloatLineEdit::FloatLineEdit()
		: LineEdit()
	{
		m_text = "0";
	}

	FloatLineEdit::FloatLineEdit(const math::Rectu& rect, 
								 const SpritePtr& defaultSprite, 
								const SpritePtr& editSprite)
				  : LineEdit(rect, defaultSprite, editSprite)
	{
	}

	bool FloatLineEdit::ProcessKeyboardInput(KeyboardEventType type, KeyCode code)
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
				if (code == KeyCode::KEY_MINUS)
					m_text += '-';
				else if (code == KeyCode::KEY_PLUS)
					m_text += '+';
				else if (code == KeyCode::KEY_PERIOD)
					m_text += '.';
				else
					m_text += (char)code;
				if (!CheckFloatFilter(m_text))
					m_text.pop_back();
				else
					m_dirty = true;
			}
				
			return true;
		}
		return false;
	}

	bool FloatLineEdit::CheckFloatFilter(const String& string)
	{
		return std::regex_match(string, std::regex("[+-]?\\d*((\\.)?(\\d*))"));
	}

	void FloatLineEdit::UpdateIfDirty()
	{
		if (m_dirty)
		{
			OnTextChanged(m_text);

			std::stringstream s(m_text);
			m_value = 0.0f;
			s >> m_value;

			OnValueChanged(m_value);
			m_dirty = false;
		}
	}

} // gui

} // sh