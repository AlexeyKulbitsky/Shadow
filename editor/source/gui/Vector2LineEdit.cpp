#include "Vector2LineEdit.h"

#include <gui/HorizontalLayout.h>
#include <gui/Widget.h>

Vector2LineEdit::Vector2LineEdit()
{
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	layout->SetSpacing(2);
	for (auto &lineEdit : m_lineEdits)
	{
		lineEdit.reset(new sh::gui::FloatLineEdit());

		layout->AddWidget(lineEdit);
	}
	SetLayout(layout);

	m_lineEdits[0]->OnValueChanged.Connect(std::bind(&Vector2LineEdit::OnXvalueChanged, this, std::placeholders::_1));
	m_lineEdits[1]->OnValueChanged.Connect(std::bind(&Vector2LineEdit::OnYvalueChanged, this, std::placeholders::_1));
}

void Vector2LineEdit::SetValue(const sh::math::Vector2f& value)
{
	m_value = value;

	m_lineEdits[0]->SetValue(value.x);
	m_lineEdits[1]->SetValue(value.y);
}

void Vector2LineEdit::OnXvalueChanged(float value)
{
	m_value.x = value;
	xValueChanged(value);
	valueChanged(m_value);
}

void Vector2LineEdit::OnYvalueChanged(float value)
{
	m_value.y = value;
	yValueChanged(value);
	valueChanged(m_value);
}
