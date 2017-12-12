#ifndef INT_PROPERTY_EDITOR_INCLUDE
#define INT_PROPERTY_EDITOR_INCLUDE

#include <Shadow.h>

class IntPropertyEditor : public sh::gui::Widget
{
public:
	IntPropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	void OnValueChanged(int value);
	void OnEnumValueChanged(sh::u32 value);

private:
	sh::Serializable* m_object = nullptr;
	sh::Property* m_objectProperty = nullptr;
};

#endif