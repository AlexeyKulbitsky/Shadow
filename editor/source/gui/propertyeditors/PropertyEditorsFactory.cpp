#include "PropertyEditorsFactory.h"

#include "Vector2PropertyEditor.h"
#include "Vector3PropertyEditor.h"
#include "Vector4PropertyEditor.h"
#include "QuaternionPropertyEditor.h"
#include "ResourceRefPropertyEditor.h"

PropertyEditorsFactory::PropertyEditorsFactory()
{
	m_creators[sh::VAR_VECTOR_2_FLOAT].reset(new ConcretePropertyEditorFactoryImpl<Vector2PropertyEditor>());
	m_creators[sh::VAR_VECTOR_3_FLOAT].reset(new ConcretePropertyEditorFactoryImpl<Vector3PropertyEditor>());
	m_creators[sh::VAR_VECTOR_4_FLOAT].reset(new ConcretePropertyEditorFactoryImpl<Vector4PropertyEditor>());
	m_creators[sh::VAR_QUATERNION_FLOAT].reset(new ConcretePropertyEditorFactoryImpl<QuaternionPropertyEditor>());
	m_creators[sh::VAR_RESOURCE_REF].reset(new ConcretePropertyEditorFactoryImpl<ResourceRefPropertyEditor>());
}

sh::gui::WidgetPtr PropertyEditorsFactory::CreatePropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
{
	auto it = m_creators.find(objectProperty->GetType());
	if (it == m_creators.end())
		return sh::gui::WidgetPtr();
	
	return it->second->CreatePropertyEditor(object, objectProperty);
}