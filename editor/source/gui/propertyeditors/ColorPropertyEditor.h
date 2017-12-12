#ifndef COLOR_PROPERTY_EDITOR_INCLUDE
#define COLOR_PROPERTY_EDITOR_INCLUDE

#include <Shadow.h>

#include "../colorpicker/ColorPicker.h"

class ColorPropertyEditor : public sh::gui::Widget
{
public:
	ColorPropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	void OnColorChanged(const sh::video::Color& color);
	void OnColorPickerButtonReleased();

private:
	sh::SPtr<ColorPicker> m_colorPicker;
	sh::gui::SpriteWidgetPtr m_colorWidget;
	sh::Serializable* m_object = nullptr;
	sh::Property* m_objectProperty = nullptr;
};

#endif
