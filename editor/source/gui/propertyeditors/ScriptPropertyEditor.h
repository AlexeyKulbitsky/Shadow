#ifndef SCRIPT_PROPERTY_EDITOR_INCLUDE
#define SCRIPT_PROPERTY_EDITOR_INCLUDE

#include <Shadow.h>

class ScriptPropertyEditor : public sh::gui::Widget
{
public:
	ScriptPropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	sh::gui::ComboBoxPtr m_comboBox;
	sh::Serializable* m_object = nullptr;
	sh::Property* m_objectProperty = nullptr;
};

#endif
