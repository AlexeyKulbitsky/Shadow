#ifndef SHADOW_EDITOR_HSVWIDGET_INCLUDE
#define SHADOW_EDITOR_HSVWIDGET_INCLUDE

#include "ColorChannelEdit.h"


class HSVWidget : public sh::gui::Widget
{
public:
	HSVWidget();

	sh::Event<void, const sh::math::Vector3Int&> colorChanged;

	const sh::math::Vector3Int& GetColor() const { return m_color; }
	void SetColor(const sh::math::Vector3Int& color);
	void SetColor(int hue, int saturation, int value);

	void OnHueChannelChanged(int value);
	void OnSaturationChannelChanged(int value);
	void OnValueChannelChanged(int value);

private:
	sh::math::Vector3Int m_color;
	std::array<sh::SPtr<ColorChannelEdit>, 3U> m_hsvChannels;
};

#endif
