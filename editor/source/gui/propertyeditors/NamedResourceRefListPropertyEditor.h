#ifndef NAMED_RESOURCE_REF_LIST_PROPERTY_EDITOR_INCLUDE
#define NAMED_RESOURCE_REF_LIST_PROPERTY_EDITOR_INCLUDE

#include <Shadow.h>

class NamedResourceRefListPropertyEditor : public sh::gui::Widget
{
public:
    NamedResourceRefListPropertyEditor(sh::Serializable* object, sh::Property* objectProperty);
    
private:
    sh::Serializable* m_object = nullptr;
    sh::Property* m_objectProperty = nullptr;
};

#endif
