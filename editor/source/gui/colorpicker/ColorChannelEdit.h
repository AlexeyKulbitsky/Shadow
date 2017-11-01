#ifndef SHADOW_COLOR_CHANNEL_EDIT_INCLUDE
#define SHADOW_COLOR_CHANNEL_EDIT_INCLUDE

#include <Shadow.h>

class ColorChannelEdit : public sh::gui::Widget
{
public:
	ColorChannelEdit(const sh::String& name, int minValue, int maxValue);
	void SetValue(int value);

	sh::Event<void, int> OnValueChanged;

private:
	void OnSliderValueChanged(float value);
	void OnEditValueChanged(int value);

	sh::math::Vector2i m_valueEdges;
	sh::gui::SliderWidgetPtr m_valueSlider;
	sh::gui::IntLineEditPtr m_valueLineEdit;
};

#endif
