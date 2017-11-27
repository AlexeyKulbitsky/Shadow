#ifndef PROPERTY_EDITORS_FACTORY_INCLUDE
#define PROPERTY_EDITORS_FACTORY_INCLUDE

#include <Shadow.h>

class ConcretePropertyEditorFactory
{
public:
	virtual sh::gui::WidgetPtr CreatePropertyEditor(sh::Serializable* object, sh::Property* objectProperty) = 0;
};

template<typename T>
class ConcretePropertyEditorFactoryImpl : public ConcretePropertyEditorFactory
{
	virtual sh::gui::WidgetPtr CreatePropertyEditor(sh::Serializable* object, sh::Property* objectProperty) override final
	{
		return sh::gui::WidgetPtr(new T(object, objectProperty));
	}
};

class PropertyEditorsFactory : public sh::Singleton<PropertyEditorsFactory>
{
public:
	PropertyEditorsFactory();
	sh::gui::WidgetPtr CreatePropertyEditor(sh::Serializable* object, sh::Property* objectProperty);

private:
	sh::Map<sh::VariantType, sh::SPtr<ConcretePropertyEditorFactory>> m_creators;
};

#endif
