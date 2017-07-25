#ifndef SHADOW_EDITOR_INSPECTOR_WIDGET_INCLUDE
#define SHADOW_EDITOR_INSPECTOR_WIDGET_INCLUDE

#include <Shadow.h>

#include "TransformWidget.h"

class InspectorWidget
{
public:
	InspectorWidget();
	~InspectorWidget();

	void SetEntity(sh::Entity* entity);
	sh::Entity* GetEntity() { return m_entity; }

	const sh::SPtr<TransformWidget>& GetTransformWidget() const { return m_transformWidget; }

private:
	sh::Entity* m_entity = nullptr;

	sh::SPtr<TransformWidget> m_transformWidget;
};

#endif
