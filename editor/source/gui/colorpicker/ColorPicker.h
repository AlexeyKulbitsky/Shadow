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

	const sh::video::Color& GetColor() const { return m_color; }
	void SetColor(const sh::video::Color& color);

	void OnRGBColorChanged(const sh::math::Vector4Int& color);
	void OnHSVColorChanged(const sh::math::Vector3Int& color);

	sh::Event<void, sh::video::Color> colorChanged;

private:
	sh::video::TexturePtr CreatePalletteTexture() const;
	sh::math::Vector3 HSVtoRGB(int h, float s, float v) const;
	void RGBtoHSV(const sh::math::Vector3& rgb, int& h, float& s, float& v);

	sh::SPtr<RGBWidget> m_rgbWidget;
	sh::SPtr<HSVWidget> m_hsvWidget;

	sh::gui::ImageWidgetPtr m_paletteWidget;
	sh::gui::SpriteWidgetPtr m_colorTarget;
	sh::math::Vector2Int m_colorTargetOffset;

	sh::video::Color m_color;
	bool m_dragStarted = false;
};

#endif
