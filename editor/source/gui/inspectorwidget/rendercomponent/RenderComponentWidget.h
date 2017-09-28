#ifndef SHADOW_EDITOR_MATERIAL_WIDGET_INCLUDE
#define SHADOW_EDITOR_MATERIAL_WIDGET_INCLUDE

#include "../../ExpandableWidget.h"

class MeshMaterialParam : public sh::gui::Widget
{
public:
	MeshMaterialParam(const sh::scene::MeshPtr& mesh);

private:
	void MaterialChanged(sh::u32 index);

private:
	std::vector<std::weak_ptr<sh::io::FileInfo>> m_materialInfos;
	sh::scene::MeshPtr m_mesh;
};

////////////////////////////////////////////////////////////////////////

class RenderComponentWidget : public ExpandableWidget
{
public:
	RenderComponentWidget();
	~RenderComponentWidget();

	//const sh::gui::WidgetPtr& GetWidget() const { return m_widget; }

	void SetRenderComponent(sh::RenderComponent* component);

private:
	void OnButtonToggled(bool toggled);
	void ModelChanged(sh::u32 index);

private:
	std::vector<std::weak_ptr<sh::io::FileInfo>> m_modelFileInfos;
	sh::RenderComponent* m_renderComponent = nullptr;
	sh::gui::WidgetPtr m_modelWidget;
	sh::gui::ComboBoxPtr m_modelComboBox;
};

#endif