#include "FloatLineEdit.h"
#include "GuiManager.h"
#include "Sprite.h"
#include "../serialization/ObjectFactory.h"

#include <iomanip>

namespace sh
{

namespace gui
{
	FloatLineEdit::FloatLineEdit()
		: LineEdit()
	{
		m_text = "0";
        m_name = "FloatLineEdit";
	}

	FloatLineEdit::FloatLineEdit(const math::Rect& rect, 
								 const SpritePtr& defaultSprite, 
								const SpritePtr& editSprite)
				  : LineEdit(rect, defaultSprite, editSprite)
	{
        m_name = "FloatLineEdit";
	}

    void FloatLineEdit::RegisterObject()
    {
        ObjectFactory::GetInstance()->RegisterFactory<FloatLineEdit>("UI");
        ObjectFactory::GetInstance()->RegisterParentProperties<FloatLineEdit, LineEdit>();
        S_ACCESSOR_PROPERTY("Value", GetValue, SetValue);
    }
    
	bool FloatLineEdit::ProcessEvent(GUIEvent& ev)
	{
		if (IsInFocus() && ev.type == EventType::KeyDown)
		{
			KeyCode keyCode = static_cast<KeyCode>(ev.keyCode);

			auto text = m_text;
			switch (keyCode)
			{
			case KeyCode::KEY_BACK:
			{
				if (text.size() >= 1)
				{
					text.erase(text.begin() + (s_cursorPos - 1));
					s_cursorPos--;
					m_text = text;
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
			case KeyCode::KEY_MINUS:
			{
				text.insert(text.begin() + s_cursorPos++, '-');
			}
			break;
			case KeyCode::KEY_PLUS:
			{
				text.insert(text.begin() + s_cursorPos++, '+');
			}
			break;
			case KeyCode::KEY_PERIOD:
			{
				text.insert(text.begin() + s_cursorPos++, '.');
			}
			break;
			default:
			{
				text.insert(text.begin() + s_cursorPos++, (char)ev.keyCode);
				if (CheckFloatFilter(text))
				{
					m_text = text;
					m_dirty = true;
				}
				else
				{
					s_cursorPos--;
				}
			}
			break;
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
