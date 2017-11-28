#ifndef VECTOR_2_PROPERTY_EDITOR_INCLUDE
#define VECTOR_2_PROPERTY_EDITOR_INCLUDE

#include "../Vector2LineEdit.h"
#include <Shadow.h>

class Vector2PropertyEditor : public sh::gui::Widget
{
public:
	Vector2PropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	void OnValueChanged(const sh::math::Vector2f& value);

private:
	sh::SPtr<Vector2LineEdit> m_edit;
	sh::Serializable* m_object = nullptr;
	sh::Property* m_objectProperty = nullptr;
};

#endif