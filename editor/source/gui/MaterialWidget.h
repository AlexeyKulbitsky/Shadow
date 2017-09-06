#ifndef SHADOW_EDITOR_MATERIAL_WIDGET_INCLUDE
#define SHADOW_EDITOR_MATERIAL_WIDGET_INCLUDE

#include <Shadow.h>

#include "Vector3LineEdit.h"

class MaterialParamVector3Editor : public sh::gui::Widget
{
public:
	MaterialParamVector3Editor(sh::video::MaterialParam* param);
	void SetValue(const sh::math::Vector3f& value);

private:
	sh::video::MaterialParam* m_param = nullptr;
};

class MaterialWidget
{
public:
	MaterialWidget();
	~MaterialWidget();

	const sh::gui::WidgetPtr& GetWidget() const { return m_widget; }

	void SetRenderComponent(sh::RenderComponent* component);

private:
	void OnButtonToggled(bool toggled);

private:
	sh::gui::ButtonPtr m_expandableButton;
	sh::gui::WidgetPtr m_widget;
	sh::gui::VerticalLayoutPtr m_layout;
	sh::RenderComponent* m_renderComponent = nullptr;
};

#endif