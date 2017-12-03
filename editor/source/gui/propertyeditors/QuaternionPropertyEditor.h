#ifndef QUATERNION_PROPERTY_EDITOR_INCLUDE
#define QUATERNION_PROPERTY_EDITOR_INCLUDE

#include "../Vector3LineEdit.h"
#include <Shadow.h>

class QuaternionPropertyEditor : public sh::gui::Widget
{
public:
	QuaternionPropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	void OnValueChanged(const sh::math::Vector3& value);

private:
	sh::SPtr<Vector3LineEdit> m_edit;
	sh::Serializable* m_object = nullptr;
	sh::Property* m_objectProperty = nullptr;
};

#endif
