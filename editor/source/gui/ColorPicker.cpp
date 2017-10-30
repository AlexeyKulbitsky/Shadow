#include "ColorPicker.h"

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

///////////////////////////////////////////////////////////////////////////////////////////////

ColorPicker::ColorPicker()
	:sh::gui::Window(sh::math::Recti(0, 100, 256, 500))	
{
	SetText("Color Picker");
	sh::gui::VerticalLayoutPtr layout(new sh::gui::VerticalLayout());
	SetLayout(layout);

	SetClosable(true);

	m_paletteWidget.reset(new sh::gui::ImageWidget());

	auto paletteTexture = CreatePalletteTexture();
	m_paletteWidget->SetTexture(paletteTexture);
	m_paletteWidget->SetMinimumHeight(paletteTexture->GetDescription().height);
	m_paletteWidget->SetMinimumWidth(paletteTexture->GetDescription().width);
	m_layout->AddWidget(m_paletteWidget);

	m_rgbaChannels[0].reset(new ColorChannelEdit("R", 0, 255));
	m_rgbaChannels[0]->OnValueChanged.Connect(std::bind(&ColorPicker::OnRedChannelChanged, this, std::placeholders::_1));
	m_rgbaChannels[1].reset(new ColorChannelEdit("G", 0, 255));
	m_rgbaChannels[1]->OnValueChanged.Connect(std::bind(&ColorPicker::OnGreenChannelGanged, this, std::placeholders::_1));
	m_rgbaChannels[2].reset(new ColorChannelEdit("B", 0, 255));
	m_rgbaChannels[2]->OnValueChanged.Connect(std::bind(&ColorPicker::OnBlueChannelChanged, this, std::placeholders::_1));
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
		auto offsets = m_colorTarget->GetRect().GetSize() / 2;
		m_colorTarget->SetPosition(ev.x - offsets.x, ev.y - offsets.y);
		m_colorTargetOffset = m_colorTarget->GetPosition() - m_paletteWidget->GetPosition();

		unsigned char data[4];

		auto size = m_paletteWidget->GetRect().GetSize() / 2;
		float x = ev.x - m_paletteWidget->GetPosition().x - size.x;
		float y = m_paletteWidget->GetRect().GetHeight() - ev.y + m_paletteWidget->GetPosition().y - size.y;
		const float length = sh::math::Sqrt(x * x + y * y);
		const float widgetLength = static_cast<float>(m_paletteWidget->GetRect().GetWidth() / 2.0f);
		if (length > widgetLength)
		{
			const float aspect = widgetLength / length;
			x *= aspect;
			y *= aspect;

			const sh::s32 eventX = static_cast<sh::s32>(x + m_paletteWidget->GetPosition().x + size.x);
			const sh::s32 eventY = static_cast<sh::s32>(m_paletteWidget->GetRect().GetHeight() + m_paletteWidget->GetPosition().y - size.y - y);

			m_colorTarget->SetPosition(eventX - offsets.x, eventY - offsets.y);
			m_colorTargetOffset = m_colorTarget->GetPosition() - m_paletteWidget->GetPosition();
		}

		x /= m_paletteWidget->GetRect().GetWidth() * 0.5f;
		y /= m_paletteWidget->GetRect().GetHeight() * 0.5f;
		float hue = 180.0f * sh::math::Atan2(y, x) / sh::math::k_pi;
		if (hue < 0.0f) hue += 360.0f;
		const float saturation = sh::math::Sqrt(x * x + y * y);
		if (saturation > 1.0f)
		{
			data[0U] = 0;
			data[1U] = 0;
			data[2U] = 0;
			data[3U] = 0;
		}
		else
		{
			const float value = 0.999f;
			auto color = HSVtoRGB(hue, saturation, value);
			data[0U] = static_cast<sh::u8>(color.x * 256.0f);
			data[1U] = static_cast<sh::u8>(color.y * 256.0f);
			data[2U] = static_cast<sh::u8>(color.z * 256.0f);
			data[3U] = 255;
		}

		m_rgbaChannels[0]->SetValue(data[0]);
		m_rgbaChannels[1]->SetValue(data[1]);
		m_rgbaChannels[2]->SetValue(data[2]);
		m_rgbaChannels[3]->SetValue(data[3]);

		m_color.x = data[0] / 256.0f;
		m_color.y = data[1] / 256.0f;
		m_color.z = data[2] / 256.0f;
		m_color.w = data[3] / 256.0f;

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

void ColorPicker::OnRedChannelChanged(int red)
{
	m_color.x = red / 256.0f;
	int hue = 0;
	float saturation = 0.0f;
	float value = 0.0f;
	sh::math::Vector3f color(m_color.x, m_color.y, m_color.z);
	RGBtoHSV(color, hue, saturation, value);

	const float r = saturation * m_paletteWidget->GetRect().GetWidth() / 2.0f;
	const sh::s32 x = static_cast<sh::s32>(r * sh::math::Cos(hue * sh::math::k_pi / 180.0f));
	const sh::s32 y = static_cast<sh::s32>(r * sh::math::Sin(hue * sh::math::k_pi / 180.0f));

	const auto pos = m_paletteWidget->GetPosition() + m_paletteWidget->GetRect().GetWidth() / 2 + sh::math::Vector2i(x, y);

	auto offsets = m_colorTarget->GetRect().GetSize() / 2;
	m_colorTarget->SetPosition(pos.x - offsets.x, pos.y - offsets.y);
	m_colorTargetOffset = m_colorTarget->GetPosition() - m_paletteWidget->GetPosition();
}

void ColorPicker::OnGreenChannelGanged(int green)
{
	m_color.y = green / 256.0f;
	int hue = 0;
	float saturation = 0.0f;
	float value = 0.0f;
	sh::math::Vector3f color(m_color.x, m_color.y, m_color.z);
	RGBtoHSV(color, hue, saturation, value);

	const float r = saturation * m_paletteWidget->GetRect().GetWidth() / 2.0f;
	const sh::s32 x = static_cast<sh::s32>(r * sh::math::Cos(hue * sh::math::k_pi / 180.0f));
	const sh::s32 y = static_cast<sh::s32>(r * sh::math::Sin(hue * sh::math::k_pi / 180.0f));

	const auto pos = m_paletteWidget->GetPosition() + m_paletteWidget->GetRect().GetWidth() / 2 + sh::math::Vector2i(x, y);

	auto offsets = m_colorTarget->GetRect().GetSize() / 2;
	m_colorTarget->SetPosition(pos.x - offsets.x, pos.y - offsets.y);
	m_colorTargetOffset = m_colorTarget->GetPosition() - m_paletteWidget->GetPosition();
}

void ColorPicker::OnBlueChannelChanged(int blue)
{
	m_color.z = blue / 256.0f;
	int hue = 0;
	float saturation = 0.0f;
	float value = 0.0f;
	sh::math::Vector3f color(m_color.x, m_color.y, m_color.z);
	RGBtoHSV(color, hue, saturation, value);

	const float r = saturation * m_paletteWidget->GetRect().GetWidth() / 2.0f;
	const sh::s32 x = static_cast<sh::s32>(r * sh::math::Cos(hue * sh::math::k_pi / 180.0f));
	const sh::s32 y = static_cast<sh::s32>(r * sh::math::Sin(hue * sh::math::k_pi / 180.0f));

	const auto pos = m_paletteWidget->GetPosition() + m_paletteWidget->GetRect().GetWidth() / 2 + sh::math::Vector2i(x, y);

	auto offsets = m_colorTarget->GetRect().GetSize() / 2;
	m_colorTarget->SetPosition(pos.x - offsets.x, pos.y - offsets.y);
	m_colorTargetOffset = m_colorTarget->GetPosition() - m_paletteWidget->GetPosition();
}

sh::video::TexturePtr ColorPicker::CreatePalletteTexture() const
{
	const sh::u32 width = 256U;
	const sh::u32 height = 256U;
	const sh::u32 channels = 4U;

	sh::u8 data[width * height * channels];

	for (sh::u32 i = 0U; i < height; ++i)
	{
		for (sh::u32 j = 0U; j < width; ++j)
		{
			const float xPixels = static_cast<float>(j) - width / 2.0f;
			const float yPixels = static_cast<float>(i) - height / 2.0f;
			const float x = xPixels / (width * 0.5f);
			const float y = yPixels / (height * 0.5f);

			float hue = 180.0f * sh::math::Atan2(y, x) / sh::math::k_pi;
			if (hue < 0.0f) hue += 360.0f;
			const float saturation = sh::math::Sqrt(x * x + y * y);
			if (saturation > 1.0f)
			{
				data[i * width * channels + j * channels] = 0;
				data[i * width * channels + j * channels + 1U] = 0;
				data[i * width * channels + j * channels + 2U] = 0;
				data[i * width * channels + j * channels + 3U] = 0;
			}
			else
			{
				const float value = 0.999f;
				auto color = HSVtoRGB(hue, saturation, value);
				data[i * width * channels + j * channels] = static_cast<sh::u8>(color.x * 256.0f);
				data[i * width * channels + j * channels + 1U] = static_cast<sh::u8>(color.y * 256.0f);
				data[i * width * channels + j * channels + 2U] = static_cast<sh::u8>(color.z * 256.0f);
				data[i * width * channels + j * channels + 3U] = 255;
			}
		}
	}

	sh::video::TextureDescription desc;
	desc.format = sh::TextureFormat::RGBA;
	desc.type = sh::TEX_TYPE_TEXTURE_2D;
	desc.width = width;
	desc.height = height;

	auto texture = sh::video::Texture::Create(desc);
	texture->SetData(0U, data);

	return texture;
}

sh::math::Vector3f ColorPicker::HSVtoRGB(int h, float s, float v) const
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

	return color;
}

void ColorPicker::RGBtoHSV(const sh::math::Vector3f& rgb, int& h, float& s, float& v)
{
	const float min = sh::math::Min(sh::math::Min(rgb.x, rgb.y), rgb.z);
	const float max = sh::math::Max(sh::math::Max(rgb.x, rgb.y), rgb.z);

	s = 1.0f - min / max;
	v = max;
	if (max == min)
	{
		h = 0;
		s = 0.0f;
	}
	else if(max == rgb.x)
	{
		if (rgb.y >= rgb.z)
		{
			h = static_cast<int>(60.0f * (rgb.y - rgb.z) / (max - min));
		}
		else
		{
			h = static_cast<int>(60.0f * (rgb.y - rgb.z) / (max - min)) + 360;
		}
	}
	else if (max == rgb.y)
	{
		h = static_cast<int>(60.0f * (rgb.z - rgb.x) / (max - min)) + 120;
	}
	else if (max == rgb.z)
	{
		h = static_cast<int>(60.0f * (rgb.x - rgb.y) / (max - min)) + 240;
	}
}