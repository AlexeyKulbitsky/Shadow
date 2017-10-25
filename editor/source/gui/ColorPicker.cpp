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
	m_valueSlider->SetMinimumWidth(180);
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
	SetText("Color Picker");
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

	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("cross");
	m_colorTarget = icon->Clone();
	m_colorTarget->SetSize(m_colorTarget->GetRect().GetSize() * 2);

	auto targetPos = m_paletteWidget->GetPosition() + sh::math::Vector2i(m_paletteWidget->GetRect().GetWidth(), m_paletteWidget->GetRect().GetHeight());
	auto offsets = m_colorTarget->GetRect().GetSize() / 2;
	m_colorTarget->SetPosition(targetPos.x - offsets.x, targetPos.y - offsets.y);
}

void ColorPicker::Render(sh::video::Painter* painter)
{
	sh::gui::Window::Render(painter);
	m_colorTarget->Render(painter);
}

bool ColorPicker::ProcessEvent(sh::gui::GUIEvent& ev)
{
	bool isInside = m_paletteWidget->GetRect().IsPointInside(ev.x, ev.y);
	bool isEventProcessed = false;

	switch (ev.type)
	{
	case sh::gui::EventType::PointerDown:
	{
		if (isInside && !m_dragStarted)
		{
			m_dragStarted = true;
			isEventProcessed = true;
		}
		else
		{
			m_dragStarted = false;
		}
	}
		break;
	case sh::gui::EventType::PointerMove:
	{
		if (isInside && m_dragStarted)
		{
			isEventProcessed = true;
		}
	}
	break;
	case sh::gui::EventType::PointerUp:
	{
		m_dragStarted = false;
		if (isInside)
		{
			isEventProcessed = true;
		}
	}
	break;
	default:
		break;
	}


	if (isEventProcessed)
	{
		sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
		auto painter = driver->GetPainter();
		driver->BeginRendering();
		m_paletteWidget->Render(painter);
		painter->Flush();

		unsigned char data[4];
		sh::Device::GetInstance()->GetDriver()->GetPixelData(ev.x, ev.y, data);

		m_rgbaChannels[0]->SetValue(data[0]);
		m_rgbaChannels[1]->SetValue(data[1]);
		m_rgbaChannels[2]->SetValue(data[2]);
		m_rgbaChannels[3]->SetValue(data[3]);

		auto offsets = m_colorTarget->GetRect().GetSize() / 2;
		m_colorTarget->SetPosition(ev.x - offsets.x, ev.y - offsets.y);

		m_colorTargetOffset = m_colorTarget->GetPosition() - m_paletteWidget->GetPosition();
		return true;
	}

	return sh::gui::Window::ProcessEvent(ev);
}

void ColorPicker::UpdateLayout()
{
	Window::UpdateLayout();

	if (!m_paletteWidget || !m_colorTarget)
		return;

	auto pos = m_paletteWidget->GetPosition() + m_colorTargetOffset;
	m_colorTarget->SetPosition(pos.x, pos.y);
}

void HSVtoRGB(int h, float s, float v)
{
	float hue = static_cast<float>(h) / 60.0f;
	int index = static_cast<int>(hue);
	float fract = hue - static_cast<float>(index);

	float p = v * (1.0f - s);
	float q = v * (1.0f - s * fract);
	float t = v * (1.0f - s * (1.0f - fract));

	sh::math::Vector3f color(0.0f);

	switch (index)
	{
	case 0:
	{
		color.x = v;
		color.y = t;
		color.z = p;
	}
		break;
	case 1:
	{
		color.x = q;
		color.y = v;
		color.z = p;
	}
		break;
	case 2:
	{
		color.x = p;
		color.y = v;
		color.z = t;
	}
		break;
	case 3:
	{
		color.x = p;
		color.y = q;
		color.z = v;
	}
		break;
	case 4:
	{
		color.x = t;
		color.y = p;
		color.z = v;
	}
		break;
	case 5:
	{
		color.x = v;
		color.y = p;
		color.z = q;
	}
		break;
	default:
		break;
	}
}