#ifndef SHADOW_EDITOR_MATERIAL_WIDGET_INCLUDE
#define SHADOW_EDITOR_MATERIAL_WIDGET_INCLUDE

#include <Shadow.h>

class MeshMaterialParam : public sh::gui::Widget
{
public:
	MeshMaterialParam(const sh::scene::MeshPtr& mesh);

private:
	void MaterialChanged(sh::u32 index);

private:
	std::vector<sh::String> m_materialNames;
	sh::scene::Mesh* m_mesh = nullptr;
};

////////////////////////////////////////////////////////////////////////

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
	sh::gui::WidgetPtr m_widget;
	sh::gui::VerticalLayoutPtr m_layout;
	sh::RenderComponent* m_renderComponent = nullptr;
};

#endif