#ifndef FLOAT_PROPERTY_EDITOR_INCLUDE
#define FLOAT_PROPERTY_EDITOR_INCLUDE

#include <Shadow.h>

class FloatPropertyEditor : public sh::gui::Widget
{
public:
	FloatPropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	void OnValueChanged(float value);

private:
	sh::Serializable* m_object = nullptr;
	sh::Property* m_objectProperty = nullptr;
};

#endif