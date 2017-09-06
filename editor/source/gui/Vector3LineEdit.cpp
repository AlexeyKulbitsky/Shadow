#include "Vector3LineEdit.h"

#include <gui/HorizontalLayout.h>
#include <gui/Widget.h>

Vector3LineEdit::Vector3LineEdit()
{
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	layout->SetSpacing(2);
	for (auto &lineEdit : m_lineEdits)
	{
		lineEdit.reset(new sh::gui::FloatLineEdit());

		layout->AddWidget(lineEdit);
	}
	SetLayout(layout);

	m_lineEdits[0]->OnValueChanged.Connect(std::bind(&Vector3LineEdit::OnXvalueChanged, this, std::placeholders::_1));
	m_lineEdits[1]->OnValueChanged.Connect(std::bind(&Vector3LineEdit::OnYvalueChanged, this, std::placeholders::_1));
	m_lineEdits[2]->OnValueChanged.Connect(std::bind(&Vector3LineEdit::OnZvalueChanged, this, std::placeholders::_1));
}

void Vector3LineEdit::SetValue(const sh::math::Vector3f& value)
{
	m_value = value;

	m_lineEdits[0]->SetValue(value.x);
	m_lineEdits[1]->SetValue(value.y);
	m_lineEdits[2]->SetValue(value.z);
}

void Vector3LineEdit::OnXvalueChanged(float value)
{
	m_value.x = value;
	xValueChanged(value);
	valueChanged(m_value);
}

void Vector3LineEdit::OnYvalueChanged(float value)
{
	m_value.y = value;
	yValueChanged(value);
	valueChanged(m_value);
}

void Vector3LineEdit::OnZvalueChanged(float value)
{
	m_value.z = value;
	zValueChanged(value);
	valueChanged(m_value);
}
