#ifndef VECTOR_4_PROPERTY_EDITOR_INCLUDE
#define VECTOR_4_PROPERTY_EDITOR_INCLUDE

#include "../Vector4LineEdit.h"
#include <Shadow.h>

class Vector4PropertyEditor : public sh::gui::Widget
{
public:
	Vector4PropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	void OnValueChanged(const sh::math::Vector4& value);

private:
	sh::SPtr<Vector4LineEdit> m_edit;
	sh::Serializable* m_object = nullptr;
	sh::Property* m_objectProperty = nullptr;
};

#endif
