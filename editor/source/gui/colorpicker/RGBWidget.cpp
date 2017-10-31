#include "RGBWidget.h"

RGBWidget::RGBWidget()
{
	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	SetLayout(layout);
	SetMinimumHeight(100);

	m_rgbaChannels[0].reset(new ColorChannelEdit("R", 0, 255));
	m_rgbaChannels[0]->OnValueChanged.Connect(std::bind(&RGBWidget::OnRedChannelChanged, this, std::placeholders::_1));
	m_rgbaChannels[1].reset(new ColorChannelEdit("G", 0, 255));
	m_rgbaChannels[1]->OnValueChanged.Connect(std::bind(&RGBWidget::OnGreenChannelGanged, this, std::placeholders::_1));
	m_rgbaChannels[2].reset(new ColorChannelEdit("B", 0, 255));
	m_rgbaChannels[2]->OnValueChanged.Connect(std::bind(&RGBWidget::OnBlueChannelChanged, this, std::placeholders::_1));
	m_rgbaChannels[3].reset(new ColorChannelEdit("A", 0, 255));

	sh::gui::LabelPtr RGBLabel(new sh::gui::Label("RGB"));

	m_layout->AddWidget(RGBLabel);
	m_layout->AddWidget(m_rgbaChannels[0]);
	m_layout->AddWidget(m_rgbaChannels[1]);
	m_layout->AddWidget(m_rgbaChannels[2]);
	m_layout->AddWidget(m_rgbaChannels[3]);
}

void RGBWidget::SetColor(const sh::math::Vector4i& color)
{
	m_color = color;
	m_rgbaChannels[0]->SetValue(color.x);
	m_rgbaChannels[1]->SetValue(color.y);
	m_rgbaChannels[2]->SetValue(color.z);
	m_rgbaChannels[3]->SetValue(color.w);
}

void RGBWidget::SetColor(sh::u8 red, sh::u8 green, sh::u8 blue, sh::u8 alpha)
{
	m_color.x = static_cast<int>(red);
	m_color.y = static_cast<int>(green);
	m_color.z = static_cast<int>(blue);
	m_color.w = static_cast<int>(alpha);
	m_rgbaChannels[0]->SetValue(m_color.x);
	m_rgbaChannels[1]->SetValue(m_color.y);
	m_rgbaChannels[2]->SetValue(m_color.z);
	m_rgbaChannels[3]->SetValue(m_color.w);
}

void RGBWidget::SetColor(const sh::math::Vector4f& color)
{
	m_color.x = static_cast<int>(color.x * 256.0f);
	m_color.y = static_cast<int>(color.y * 256.0f);
	m_color.z = static_cast<int>(color.z * 256.0f);
	m_color.w = static_cast<int>(color.w * 256.0f);
	m_rgbaChannels[0]->SetValue(m_color.x);
	m_rgbaChannels[1]->SetValue(m_color.y);
	m_rgbaChannels[2]->SetValue(m_color.z);
	m_rgbaChannels[3]->SetValue(m_color.w);
}

void RGBWidget::OnRedChannelChanged(int red)
{
	m_color.x = red;
	colorChanged(m_color);
}

void RGBWidget::OnGreenChannelGanged(int green)
{
	m_color.y = green;
	colorChanged(m_color);
}

void RGBWidget::OnBlueChannelChanged(int blue)
{
	m_color.z = blue;
	colorChanged(m_color);
}