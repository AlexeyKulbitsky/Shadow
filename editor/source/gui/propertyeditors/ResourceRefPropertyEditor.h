#ifndef RESOURCE_REF_PROPERTY_EDITOR_INCLUDE
#define RESOURCE_REF_PROPERTY_EDITOR_INCLUDE

#include <Shadow.h>

class ResourceRefPropertyEditor : public sh::gui::Widget
{
public:
	ResourceRefPropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	void OnRefChanged(sh::u32 index);

private:
	sh::gui::ComboBoxPtr m_comboBox;
	sh::Serializable* m_object = nullptr;
	sh::Property* m_objectProperty = nullptr;
};

#endif
