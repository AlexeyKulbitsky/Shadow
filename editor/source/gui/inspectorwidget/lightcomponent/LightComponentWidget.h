#ifndef SHADOW_LIGHT_COMPONENT_WIDGET_INCLUDE
#define SHADOW_LIGHT_COMPONENT_WIDGET_INCLUDE

#include "../../ExpandableWidget.h"
#include "../../ColorPicker.h"

class LightComponentWidget : public ExpandableWidget
{
public:
	LightComponentWidget();

	void SetLightComponent(sh::LightComponent* component);

	void OnColorPickerButtonReleased();

private:
	sh::LightComponent* m_lightComponent = nullptr;

	sh::SPtr<ColorPicker> m_colorPicker;
};

#endif