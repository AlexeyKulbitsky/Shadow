#ifndef SHADOW_LIGHT_COMPONENT_WIDGET_INCLUDE
#define SHADOW_LIGHT_COMPONENT_WIDGET_INCLUDE

#include "../../ExpandableWidget.h"
#include "../../colorpicker/ColorPicker.h"

class LightComponentWidget : public ExpandableWidget
{
public:
	LightComponentWidget();

	void SetLightComponent(sh::LightComponent* component);

	void OnColorPickerButtonReleased();

	void OnColorChanged(const sh::math::Vector4f& color);

private:
	sh::LightComponent* m_lightComponent = nullptr;

	sh::SPtr<ColorPicker> m_colorPicker;
	sh::gui::SpriteWidgetPtr m_colorWidget;
};

#endif