#include "ColorChannelEdit.h"

ColorChannelEdit::ColorChannelEdit(const sh::String& name, int minValue, int maxValue)
{
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	layout->SetSpacing(5);

	sh::gui::LabelPtr nameLabel(new sh::gui::Label(name));
	nameLabel->SetMaximumWidth(20);
	m_valueLineEdit.reset(new sh::gui::IntLineEdit());
	m_valueLineEdit->SetValue(0);
	m_valueSlider.reset(new sh::gui::SliderWidget());
	m_valueSlider->SetMinimumWidth(180);
	m_valueSlider->SetMinValue(static_cast<float>(minValue));
	m_valueSlider->SetMaxValue(static_cast<float>(maxValue));
	m_valueSlider->valueChanged.Connect(std::bind(&ColorChannelEdit::OnSliderValueChanged, this, std::placeholders::_1));
	m_valueLineEdit->OnValueChanged.Connect(std::bind(&ColorChannelEdit::OnEditValueChanged, this, std::placeholders::_1));


	layout->SetMargins(2, 2, 2, 2);
	layout->AddWidget(nameLabel);
	layout->AddWidget(m_valueSlider);
	layout->AddWidget(m_valueLineEdit);
	sh::gui::WidgetPtr itemWidget(new sh::gui::Widget());
	SetMaximumHeight(20);
	SetLayout(layout);
}

void ColorChannelEdit::SetValue(int value)
{
	m_valueSlider->SetValue(static_cast<float>(value));
	m_valueLineEdit->SetValue(value);
}

void ColorChannelEdit::OnSliderValueChanged(float value)
{
	int intValue = static_cast<int>(value);
	m_valueLineEdit->SetValue(value);
	OnValueChanged(value);
}

void ColorChannelEdit::OnEditValueChanged(int value)
{
	m_valueSlider->SetValue(static_cast<float>(value));
	OnValueChanged(value);
}