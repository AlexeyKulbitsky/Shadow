#ifndef SHADOW_EDITOR_RGBWIDGET_INCLUDE
#define SHADOW_EDITOR_RGBWIDGET_INCLUDE

#include "ColorChannelEdit.h"


class RGBWidget : public sh::gui::Widget
{
public:
	RGBWidget();

	sh::Event<void, const sh::math::Vector4Int&> colorChanged;

	const sh::math::Vector4Int& GetColor() const { return m_color; }
	void SetColor(const sh::math::Vector4Int& color);
	void SetColor(sh::u8 red, sh::u8 green, sh::u8 blue, sh::u8 alpha);
	void SetColor(const sh::math::Vector4& color);

	void OnRedChannelChanged(int value);
	void OnGreenChannelGanged(int value);
	void OnBlueChannelChanged(int value);

private:
	sh::math::Vector4Int m_color;
	std::array<sh::SPtr<ColorChannelEdit>, 4U> m_rgbaChannels;
};

#endif
