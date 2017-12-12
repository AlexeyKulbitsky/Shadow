#include "PropertyEditorsFactory.h"

#include "IntPropertyEditor.h"
#include "FloatPropertyEditor.h"
#include "Vector2PropertyEditor.h"
#include "Vector3PropertyEditor.h"
#include "Vector4PropertyEditor.h"
#include "ColorPropertyEditor.h"
#include "QuaternionPropertyEditor.h"
#include "ResourceRefPropertyEditor.h"
#include "ScriptPropertyEditor.h"

PropertyEditorsFactory::PropertyEditorsFactory()
{
	m_creators[sh::VAR_INT].reset(new ConcretePropertyEditorFactoryImpl<IntPropertyEditor>());
	m_creators[sh::VAR_FLOAT].reset(new ConcretePropertyEditorFactoryImpl<FloatPropertyEditor>());
	m_creators[sh::VAR_VECTOR_2_FLOAT].reset(new ConcretePropertyEditorFactoryImpl<Vector2PropertyEditor>());
	m_creators[sh::VAR_VECTOR_3_FLOAT].reset(new ConcretePropertyEditorFactoryImpl<Vector3PropertyEditor>());
	m_creators[sh::VAR_VECTOR_4_FLOAT].reset(new ConcretePropertyEditorFactoryImpl<Vector4PropertyEditor>());
	m_creators[sh::VAR_COLOR].reset(new ConcretePropertyEditorFactoryImpl<ColorPropertyEditor>());
	m_creators[sh::VAR_QUATERNION_FLOAT].reset(new ConcretePropertyEditorFactoryImpl<QuaternionPropertyEditor>());
	m_creators[sh::VAR_RESOURCE_REF].reset(new ConcretePropertyEditorFactoryImpl<ResourceRefPropertyEditor>());
	m_creators[sh::VAR_SCRIPT_REF].reset(new ConcretePropertyEditorFactoryImpl<ScriptPropertyEditor>());
}

sh::gui::WidgetPtr PropertyEditorsFactory::CreatePropertyEditor(sh::Serializable* object, sh::Property* objectProperty)
{
	auto it = m_creators.find(objectProperty->GetType());
	if (it == m_creators.end())
		return sh::gui::WidgetPtr();
	
	return it->second->CreatePropertyEditor(object, objectProperty);
}