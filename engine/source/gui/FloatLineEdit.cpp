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

	FloatLineEdit::FloatLineEdit(const math::Recti& rect, 
								 const SpritePtr& defaultSprite, 
								const SpritePtr& editSprite)
				  : LineEdit(rect, defaultSprite, editSprite)
	{
	}

	bool FloatLineEdit::ProcessEvent(GUIEvent& ev)
	{
		if (m_inFocus && ev.type == EventType::KeyDown)
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
				if (ev.keyCode == (int)KeyCode::KEY_MINUS)
					m_text += '-';
				else if (ev.keyCode == (int)KeyCode::KEY_PLUS)
					m_text += '+';
				else if (ev.keyCode == (int)KeyCode::KEY_PERIOD)
					m_text += '.';
				else
					m_text += (char)ev.keyCode;
				if (!CheckFloatFilter(m_text))
					m_text.pop_back();
				else
					m_dirty = true;
			}

			return true;
		}
		else
		{
			return LineEdit::ProcessEvent(ev);
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