#ifndef SHADOW_EDITOR_INSPECTOR_WIDGET_INCLUDE
#define SHADOW_EDITOR_INSPECTOR_WIDGET_INCLUDE

#include <Shadow.h>

#include "TransformWidget.h"
#include "MaterialWidget.h"

class InspectorWidget
{
public:
	InspectorWidget();
	~InspectorWidget();

	void SetEntity(sh::Entity* entity);
	sh::Entity* GetEntity() { return m_entity; }

	const sh::SPtr<TransformWidget>& GetTransformWidget() const { return m_transformWidget; }

private:
	void OnWindowResized(sh::s32 width, sh::s32 height);
	void UpdateGeometry(sh::u32 screenWidth, sh::u32 screenHeight);

private:
	sh::Entity* m_entity = nullptr;

	sh::SPtr<TransformWidget> m_transformWidget;
	sh::SPtr<MaterialWidget> m_materialWidget;
	sh::gui::WindowPtr m_window;
};

#endif
