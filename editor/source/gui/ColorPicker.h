#ifndef SHDOW_EDITOR_COLOR_PICKER_INCLUDE
#define SHDOW_EDITOR_COLOR_PICKER_INCLUDE

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


	sh::gui::SliderWidgetPtr m_valueSlider;
	sh::gui::IntLineEditPtr m_valueLineEdit;
};

/////////////////////////////////////////////////////////////////////////

class ColorPicker : public sh::gui::Window
{
public:
	ColorPicker();
	virtual void Render(sh::video::Painter* painter) override;
	virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;
	virtual void UpdateLayout() override;

	const sh::math::Vector4f& GetColor() const { return m_color; }

	void OnRedChannelChanged(int value);
	void OnGreenChannelGanged(int value);
	void OnBlueChannelChanged(int value);

private:
	sh::video::TexturePtr CreatePalletteTexture() const;
	sh::math::Vector3f HSVtoRGB(int h, float s, float v) const;
	void RGBtoHSV(const sh::math::Vector3f& rgb, int& h, float& s, float& v);

	std::array<sh::SPtr<ColorChannelEdit>, 4U> m_rgbaChannels;
	sh::gui::ImageWidgetPtr m_paletteWidget;
	sh::gui::SpriteWidgetPtr m_colorTarget;
	sh::math::Vector2i m_colorTargetOffset;

	sh::math::Vector4f m_color;
	bool m_dragStarted = false;
};

#endif
