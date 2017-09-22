#include "Vector4LineEdit.h"

#include <gui/HorizontalLayout.h>
#include <gui/Widget.h>

Vector4LineEdit::Vector4LineEdit()
{
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	layout->SetSpacing(2);
	for (auto &lineEdit : m_lineEdits)
	{
		lineEdit.reset(new sh::gui::FloatLineEdit());

		layout->AddWidget(lineEdit);
	}
	SetLayout(layout);

	m_lineEdits[0]->OnValueChanged.Connect(std::bind(&Vector4LineEdit::OnXvalueChanged, this, std::placeholders::_1));
	m_lineEdits[1]->OnValueChanged.Connect(std::bind(&Vector4LineEdit::OnYvalueChanged, this, std::placeholders::_1));
	m_lineEdits[2]->OnValueChanged.Connect(std::bind(&Vector4LineEdit::OnZvalueChanged, this, std::placeholders::_1));
	m_lineEdits[3]->OnValueChanged.Connect(std::bind(&Vector4LineEdit::OnWvalueChanged, this, std::placeholders::_1));
}

void Vector4LineEdit::SetValue(const sh::math::Vector4f& value)
{
	m_value = value;

	m_lineEdits[0]->SetValue(value.x);
	m_lineEdits[1]->SetValue(value.y);
	m_lineEdits[2]->SetValue(value.z);
	m_lineEdits[3]->SetValue(value.w);
}

void Vector4LineEdit::OnXvalueChanged(float value)
{
	m_value.x = value;
	xValueChanged(value);
	valueChanged(m_value);
}

void Vector4LineEdit::OnYvalueChanged(float value)
{
	m_value.y = value;
	yValueChanged(value);
	valueChanged(m_value);
}

void Vector4LineEdit::OnZvalueChanged(float value)
{
	m_value.z = value;
	zValueChanged(value);
	valueChanged(m_value);
}

void Vector4LineEdit::OnWvalueChanged(float value)
{
	m_value.w = value;
	wValueChanged(value);
	valueChanged(m_value);
}
