#ifndef VECTOR_3_PROPERTY_EDITOR_INCLUDE
#define VECTOR_3_PROPERTY_EDITOR_INCLUDE

#include "../Vector3LineEdit.h"
#include <Shadow.h>

class Vector3PropertyEditor : public sh::gui::Widget
{
public:
	Vector3PropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	void OnValueChanged(const sh::math::Vector3f& value);

private:
	sh::SPtr<Vector3LineEdit> m_edit;
	sh::Serializable* m_object = nullptr;
	sh::Property* m_objectProperty = nullptr;
};

#endif