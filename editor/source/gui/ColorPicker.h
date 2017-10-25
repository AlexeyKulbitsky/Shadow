#ifndef SHDOW_EDITOR_COLOR_PICKER_INCLUDE
#define SHDOW_EDITOR_COLOR_PICKER_INCLUDE

#include <Shadow.h>

class ColorChannelEdit : public sh::gui::Widget
{
public:
	ColorChannelEdit(const sh::String& name, int minValue, int maxValue);
	void SetValue(int value);

private:
	void OnSliderValueChanged(float value);

	sh::gui::SliderWidgetPtr m_valueSlider;
	sh::gui::LineEditPtr m_valueLineEdit;
};

class ColorPicker : public sh::gui::Window
{
public:
	ColorPicker();
	virtual void Render(sh::video::Painter* painter) override;
	virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;
	virtual void UpdateLayout() override;

private:
	std::array<sh::SPtr<ColorChannelEdit>, 4U> m_rgbaChannels;
	sh::gui::ImageWidgetPtr m_paletteWidget;
	sh::gui::SpriteWidgetPtr m_colorTarget;
	sh::math::Vector2i m_colorTargetOffset;
	bool m_dragStarted = false;
};

#endif
