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

	const sh::SPtr<TransformComponentWidget>& GetTransformWidget() const { return m_transformComponentWidget; }

private:
	sh::Entity* m_entity = nullptr;

	sh::SPtr<TransformComponentWidget> m_transformComponentWidget;
	sh::SPtr<RenderComponentWidget> m_renderComponentWidget;
	sh::SPtr<LightComponentWidget> m_lightComponentWidget;
};

#endif
