#include "FloatLineEdit.h"

#include "Sprite.h"

#include <iomanip>

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
				//UpdateUV(m_defaultSprite->GetUVRect().upperLeftCorner, 
				//			m_defaultSprite->GetUVRect().lowerRightCorner);
				//UpdateColor(m_defaultSprite->GetColor());
				m_inFocus = false;
				m_state = State::Default;
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

	void FloatLineEdit::SetValue(float value) 
	{ 
		m_value = value; 

		std::stringstream s;
		s << std::fixed << std::setprecision(3) << m_value;
		SetText(s.str());

		//double pi = 3.1415926;
		//std::string s(16, '\0');
		//auto written = std::snprintf(&s[0], s.size(), "%.2f", pi);
		//s.resize(written);
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