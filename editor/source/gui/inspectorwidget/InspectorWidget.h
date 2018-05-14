#ifndef SHADOW_EDITOR_INSPECTOR_WIDGET_INCLUDE
#define SHADOW_EDITOR_INSPECTOR_WIDGET_INCLUDE

#include <Shadow.h>

#include "transformcomponent/TransformComponentWidget.h"
#include "rendercomponent/RenderComponentWidget.h"
#include "lightcomponent/LightComponentWidget.h"

class InspectorWidget : public sh::gui::Window
{
public:
	InspectorWidget();
	~InspectorWidget();

	void SetEntity(sh::Entity* entity);
	sh::Entity* GetEntity() { return m_entity; }

private:
	sh::Entity* m_entity = nullptr;
};

#endif
