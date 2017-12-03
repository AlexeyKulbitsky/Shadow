#include "HSVWidget.h"

HSVWidget::HSVWidget()
{
	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	SetLayout(layout);
	SetMinimumHeight(100);

	m_hsvChannels[0].reset(new ColorChannelEdit("H", 0, 360));
	m_hsvChannels[0]->OnValueChanged.Connect(std::bind(&HSVWidget::OnHueChannelChanged, this, std::placeholders::_1));
	m_hsvChannels[1].reset(new ColorChannelEdit("S", 0, 100));
	m_hsvChannels[1]->OnValueChanged.Connect(std::bind(&HSVWidget::OnSaturationChannelChanged, this, std::placeholders::_1));
	m_hsvChannels[2].reset(new ColorChannelEdit("V", 0, 100));
	m_hsvChannels[2]->OnValueChanged.Connect(std::bind(&HSVWidget::OnValueChannelChanged, this, std::placeholders::_1));

	sh::gui::LabelPtr HSVLabel(new sh::gui::Label("HSV"));

	m_layout->AddWidget(HSVLabel);
	m_layout->AddWidget(m_hsvChannels[0]);
	m_layout->AddWidget(m_hsvChannels[1]);
	m_layout->AddWidget(m_hsvChannels[2]);
}

void HSVWidget::SetColor(const sh::math::Vector3Int& color)
{
	m_color = color;
	m_hsvChannels[0]->SetValue(color.x);
	m_hsvChannels[1]->SetValue(color.y);
	m_hsvChannels[2]->SetValue(color.z);
}

void HSVWidget::SetColor(int hue, int saturation, int value)
{
	m_color.x = hue;
	m_color.y = saturation;
	m_color.z = value;
	m_hsvChannels[0]->SetValue(hue);
	m_hsvChannels[1]->SetValue(saturation);
	m_hsvChannels[2]->SetValue(value);
}

void HSVWidget::OnHueChannelChanged(int red)
{
	m_color.x = red;
	colorChanged(m_color);
}

void HSVWidget::OnSaturationChannelChanged(int green)
{
	m_color.y = green;
	colorChanged(m_color);
}

void HSVWidget::OnValueChannelChanged(int blue)
{
	m_color.z = blue;
	colorChanged(m_color);
}