#ifndef SHADOW_EDITOR_MATERIAL_WIDGET_INCLUDE
#define SHADOW_EDITOR_MATERIAL_WIDGET_INCLUDE

#include <Shadow.h>

class MaterialWidget
{
public:
	MaterialWidget();
	~MaterialWidget();

	void SetRenderComponent(sh::RenderComponent* component);

private:
	void OnButtonToggled(bool toggled);

private:
	sh::RenderComponent* m_renderComponent = nullptr;
};

#endif