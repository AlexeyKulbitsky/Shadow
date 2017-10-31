#ifndef SHDOW_EDITOR_COLOR_PICKER_INCLUDE
#define SHDOW_EDITOR_COLOR_PICKER_INCLUDE

#include "ColorChannelEdit.h"
#include "RGBWidget.h"
#include "HSVWidget.h"

class ColorPicker : public sh::gui::Window
{
public:
	ColorPicker();
	virtual void Render(sh::video::Painter* painter) override;
	virtual bool ProcessEvent(sh::gui::GUIEvent& ev) override;
	virtual void UpdateLayout() override;

	const sh::math::Vector4f& GetColor() const { return m_color; }
	void SetColor(const sh::math::Vector4f& color);

	void OnRGBColorChanged(const sh::math::Vector4i& color);
	void OnHSVColorChanged(const sh::math::Vector3i& color);


private:
	sh::video::TexturePtr CreatePalletteTexture() const;
	sh::math::Vector3f HSVtoRGB(int h, float s, float v) const;
	void RGBtoHSV(const sh::math::Vector3f& rgb, int& h, float& s, float& v);

	sh::SPtr<RGBWidget> m_rgbWidget;
	sh::SPtr<HSVWidget> m_hsvWidget;

	sh::gui::ImageWidgetPtr m_paletteWidget;
	sh::gui::SpriteWidgetPtr m_colorTarget;
	sh::math::Vector2i m_colorTargetOffset;

	sh::math::Vector4f m_color;
	bool m_dragStarted = false;
};

#endif
