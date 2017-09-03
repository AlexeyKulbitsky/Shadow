#include "MaterialWidget.h"

MaterialWidget::MaterialWidget()
{
	sh::gui::ButtonPtr button(new sh::gui::Button("Material"));
	button->SetToggleable(true);
	button->OnToggle.Connect(std::bind(&MaterialWidget::OnButtonToggled, this, 
		std::placeholders::_1));

	m_layout.reset(new sh::gui::VerticalLayout());
	m_layout->SetMargins(2, 2, 2, 2);
	m_layout->SetSpacing(2);
	m_layout->AddWidget(button);

	m_widget.reset(new sh::gui::Widget());
	//m_widget->SetMaximumHeight(80);
	m_widget->SetLayout(m_layout);
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
	m_layout->Clear();
	const auto& params = component->GetModel()->GetMesh(0U)->GetMaterial()->GetParams();
	sh::u32 paramsCount = params->GetParamsCount();
	for (sh::u32 i = 0; i < paramsCount; ++i)
	{
		auto param = params->GetParam(i);
		sh::gui::ButtonPtr button(new sh::gui::Button("param"));
		m_layout->AddWidget(button);
	}
}

void MaterialWidget::OnButtonToggled(bool toggled)
{

}