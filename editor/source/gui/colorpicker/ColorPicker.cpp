#include "ColorPicker.h"

///////////////////////////////////////////////////////////////////////////////////////////////

ColorPicker::ColorPicker()
	:sh::gui::Window(sh::math::Recti(0, 100, 256, 600))	
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

	m_rgbWidget.reset(new RGBWidget());
	m_rgbWidget->colorChanged.Connect(std::bind(&ColorPicker::OnRGBColorChanged, this, std::placeholders::_1));
	m_layout->AddWidget(m_rgbWidget);

	m_hsvWidget.reset(new HSVWidget());
	m_hsvWidget->colorChanged.Connect(std::bind(&ColorPicker::OnHSVColorChanged, this, std::placeholders::_1));
	m_layout->AddWidget(m_hsvWidget);


	auto icon = sh::gui::GuiManager::GetInstance()->GetStyle()->GetSpriteWidget("cross");
	m_colorTarget = icon->Clone();
	m_colorTarget->SetSize(m_colorTarget->GetRect().GetSize() * 2);

	auto targetPos = m_paletteWidget->GetPosition() + sh::math::Vector2i(m_paletteWidget->GetRect().GetWidth(), m_paletteWidget->GetRect().GetHeight());
	auto offsets = m_colorTarget->GetRect().GetSize() / 2;
	m_colorTarget->SetPosition(targetPos.x - offsets.x, targetPos.y - offsets.y);

	SetColor(sh::math::Vector4f(1.0f));
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
		if (hue < 0.0f)
			hue *= -1.0f;
		else
			hue = 360.0f - hue;
		//hue += 360.0f;
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
			const float value = m_hsvWidget->GetColor().z * 0.01f;
			auto color = HSVtoRGB(hue, saturation, value);
			data[0U] = static_cast<sh::u8>(color.x * 256.0f);
			data[1U] = static_cast<sh::u8>(color.y * 256.0f);
			data[2U] = static_cast<sh::u8>(color.z * 256.0f);
			data[3U] = 255;
		}

		m_rgbWidget->SetColor(data[0], data[1], data[2], data[3]);
		m_hsvWidget->SetColor(static_cast<int>(hue), static_cast<int>(saturation * 100.0f), m_hsvWidget->GetColor().z);

		m_color.x = data[0] / 256.0f;
		m_color.y = data[1] / 256.0f;
		m_color.z = data[2] / 256.0f;
		m_color.w = data[3] / 256.0f;

		colorChanged(m_color);

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

void ColorPicker::SetColor(const sh::math::Vector4f& _color)
{
	m_color = _color;
	m_rgbWidget->SetColor(m_color);

	int hue = 0;
	float saturation = 0.0f;
	float value = 0.0f;
	sh::math::Vector3f color(m_color.x, m_color.y, m_color.z);
	RGBtoHSV(color, hue, saturation, value);
	m_hsvWidget->SetColor(hue, static_cast<int>(saturation * 100.0f), static_cast<int>(value * 100.0f));


	const float r = saturation * m_paletteWidget->GetRect().GetWidth() / 2.0f;
	const sh::s32 x = static_cast<sh::s32>(r * sh::math::Cos(hue * sh::math::k_pi / 180.0f));
	const sh::s32 y = static_cast<sh::s32>(r * sh::math::Sin(hue * sh::math::k_pi / 180.0f));

	const auto pos = m_paletteWidget->GetPosition() + m_paletteWidget->GetRect().GetWidth() / 2 + sh::math::Vector2i(x, y);

	auto offsets = m_colorTarget->GetRect().GetSize() / 2;
	m_colorTarget->SetPosition(pos.x - offsets.x, pos.y - offsets.y);
	m_colorTargetOffset = m_colorTarget->GetPosition() - m_paletteWidget->GetPosition();
}

void ColorPicker::OnRGBColorChanged(const sh::math::Vector4i& _color)
{
	m_color.x = _color.x / 256.0f;
	m_color.y = _color.y / 256.0f;
	m_color.z = _color.z / 256.0f;
	m_color.w = _color.w / 256.0f;

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

	m_hsvWidget->SetColor(hue, static_cast<int>(saturation * 100.0f), static_cast<int>(value * 100.0f));

	colorChanged(m_color);
}

void ColorPicker::OnHSVColorChanged(const sh::math::Vector3i& _color)
{
	auto color = HSVtoRGB(_color.x, _color.y * 0.01f, _color.z * 0.01f);
	m_color.x = color.x;
	m_color.y = color.y;
	m_color.z = color.z;

	m_rgbWidget->SetColor(m_color);

	const float r = _color.y * 0.01f * m_paletteWidget->GetRect().GetWidth() / 2.0f;
	const sh::s32 x = static_cast<sh::s32>(r * sh::math::Cos(_color.x * sh::math::k_pi / 180.0f));
	const sh::s32 y = static_cast<sh::s32>(r * sh::math::Sin(_color.x * sh::math::k_pi / 180.0f));

	const auto pos = m_paletteWidget->GetPosition() + m_paletteWidget->GetRect().GetWidth() / 2 + sh::math::Vector2i(x, y);

	auto offsets = m_colorTarget->GetRect().GetSize() / 2;
	m_colorTarget->SetPosition(pos.x - offsets.x, pos.y - offsets.y);
	m_colorTargetOffset = m_colorTarget->GetPosition() - m_paletteWidget->GetPosition();

	colorChanged(m_color);
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