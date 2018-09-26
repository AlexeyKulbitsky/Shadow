#ifndef STRING_PROPERTY_EDITOR_INCLUDE
#define STRING_PROPERTY_EDITOR_INCLUDE

#include <Shadow.h>

class StringPropertyEditor : public sh::gui::Widget
{
public:
    StringPropertyEditor(sh::Serializable* object, sh::Property* objectProperty);
    
private:
    void OnValueChanged(const sh::String& value);
    
private:
    sh::Serializable* m_object = nullptr;
    sh::Property* m_objectProperty = nullptr;
};

#endif
