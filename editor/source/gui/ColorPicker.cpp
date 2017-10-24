#include "ColorPicker.h"

ColorChannelEdit::ColorChannelEdit(const sh::String& name, int minValue, int maxValue)
{
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	layout->SetSpacing(5);

	sh::gui::LabelPtr nameLabel(new sh::gui::Label(name));
	nameLabel->SetMaximumWidth(20);
	m_valueLineEdit.reset(new sh::gui::LineEdit());
	m_valueLineEdit->SetText("0");
	m_valueSlider.reset(new sh::gui::SliderWidget());
	m_valueSlider->SetMinimumWidth(190);
	m_valueSlider->SetMinValue(static_cast<float>(minValue));
	m_valueSlider->SetMaxValue(static_cast<float>(maxValue));
	m_valueSlider->valueChanged.Connect(std::bind(&ColorChannelEdit::OnSliderValueChanged, this, std::placeholders::_1));

	layout->SetMargins(2, 2, 2, 2);
	layout->AddWidget(nameLabel);
	layout->AddWidget(m_valueSlider);
	layout->AddWidget(m_valueLineEdit);
	sh::gui::WidgetPtr itemWidget(new sh::gui::Widget());
	SetMaximumHeight(20);
	SetLayout(layout);

	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("cross");
	auto clonedIcon = icon->Clone();
}

void ColorChannelEdit::SetValue(int value)
{
	m_valueSlider->SetValue(static_cast<float>(value));
	m_valueLineEdit->SetText(std::to_string(value));
}

void ColorChannelEdit::OnSliderValueChanged(float value)
{
	int intValue = static_cast<int>(value);
	m_valueLineEdit->SetText(std::to_string(intValue));
}

///////////////////////////////////////////////////////////////////////////////////////////////

ColorPicker::ColorPicker()
	:sh::gui::Window(sh::math::Recti(0, 100, 256, 500))	
{
	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	SetLayout(layout);

	SetClosable(true);

	m_paletteWidget.reset(new sh::gui::ImageWidget());
	auto paletteTexture = sh::Device::GetInstance()->GetResourceManager()->GetTexture("HSVPalette.png");
	m_paletteWidget->SetTexture(paletteTexture);
	m_paletteWidget->SetMinimumHeight(paletteTexture->GetDescription().height);
	m_paletteWidget->SetMinimumWidth(paletteTexture->GetDescription().width);
	m_layout->AddWidget(m_paletteWidget);

	m_rgbaChannels[0].reset(new ColorChannelEdit("R", 0, 255));
	m_rgbaChannels[1].reset(new ColorChannelEdit("G", 0, 255));
	m_rgbaChannels[2].reset(new ColorChannelEdit("B", 0, 255));
	m_rgbaChannels[3].reset(new ColorChannelEdit("A", 0, 255));

	m_layout->AddWidget(m_rgbaChannels[0]);
	m_layout->AddWidget(m_rgbaChannels[1]);
	m_layout->AddWidget(m_rgbaChannels[2]);
	m_layout->AddWidget(m_rgbaChannels[3]);
}

bool ColorPicker::ProcessEvent(sh::gui::GUIEvent& ev)
{
	if (ev.type == sh::gui::EventType::PointerDown &&
		m_paletteWidget->GetRect().IsPointInside(sh::math::Vector2i(ev.x, ev.y)))
	{
		unsigned char data[4];
		sh::Device::GetInstance()->GetDriver()->GetPixelData(ev.x, ev.y, data);

		m_rgbaChannels[0]->SetValue(data[0]);
		m_rgbaChannels[1]->SetValue(data[1]);
		m_rgbaChannels[2]->SetValue(data[2]);
		m_rgbaChannels[3]->SetValue(data[3]);
		return true;
	}

	return sh::gui::Window::ProcessEvent(ev);
}