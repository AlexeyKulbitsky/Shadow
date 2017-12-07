#ifndef SCRIPT_PROPERTY_EDITOR_INCLUDE
#define SCRIPT_PROPERTY_EDITOR_INCLUDE

#include <Shadow.h>

class ScriptPropertyEditor : public sh::gui::Widget
{
public:
	ScriptPropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	void OnScriptSelected(sh::u32 itemIndex);
	void UpdateScriptProperties();

private:
	sh::gui::ComboBoxPtr m_comboBox;
	sh::Serializable* m_object = nullptr;
	sh::Property* m_objectProperty = nullptr;

	sh::Script* m_script;
	sh::gui::VerticalLayoutPtr m_contentLayout;
};

#endif
