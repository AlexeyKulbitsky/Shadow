#include "MaterialWidget.h"

MaterialWidget::MaterialWidget()
{
	sh::gui::ButtonPtr button(new sh::gui::Button("Material"));
	button->SetToggleable(true);
	button->OnToggle.Connect(std::bind(&MaterialWidget::OnButtonToggled, this, 
		std::placeholders::_1));
}

MaterialWidget::~MaterialWidget()
{

}

void MaterialWidget::SetRenderComponent(sh::RenderComponent* component)
{
	m_renderComponent = component;

	if (!component)
	{
		return;
	}
}

void MaterialWidget::OnButtonToggled(bool toggled)
{

}