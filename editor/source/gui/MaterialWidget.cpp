#include "MaterialWidget.h"

MaterialParamVector3Editor::MaterialParamVector3Editor(sh::video::MaterialParam* param) : m_param(param) 
{
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(m_param->GetName()));
	label->SetMaximumWidth(75U);
	layout->AddWidget(label);
	sh::SPtr<Vector3LineEdit> editWidget(new Vector3LineEdit());
	layout->AddWidget(editWidget);
	SetLayout(layout);
	SetMaximumHeight(20);

	sh::math::Vector3f value;
	m_param->Get(value);
	editWidget->SetValue(value);

	editWidget->valueChanged.Connect(
		std::bind(&MaterialParamVector3Editor::SetValue, this, std::placeholders::_1));
}

void MaterialParamVector3Editor::SetValue(const sh::math::Vector3f& value)
{
	if (m_param)
		m_param->Set(value);
}

///////////////////////////////////////////////////////////////////////////////////


MaterialWidget::MaterialWidget()
{
	m_expandableButton.reset(new sh::gui::Button("Material"));
	m_expandableButton->SetToggleable(true);
	m_expandableButton->SetMaximumHeight(20);
	m_expandableButton->OnToggle.Connect(std::bind(&MaterialWidget::OnButtonToggled, this, 
		std::placeholders::_1));

	m_layout.reset(new sh::gui::VerticalLayout());
	m_layout->SetMargins(2, 2, 2, 2);
	m_layout->SetSpacing(2);

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
	m_layout->AddWidget(m_expandableButton);
	const auto& params = component->GetModel()->GetMesh(0U)->GetMaterial()->GetParams();
	sh::u32 paramsCount = params->GetParamsCount();
	for (sh::u32 i = 0; i < paramsCount; ++i)
	{
		auto param = params->GetParam(i);

		sh::gui::WidgetPtr widget;
		switch (param->GetType())
		{
			case sh::MaterialParamType::Float3:
				widget.reset(new MaterialParamVector3Editor(param));
				break;
			default:
				break;
		}
		m_layout->AddWidget(widget);
	}
}

void MaterialWidget::OnButtonToggled(bool toggled)
{

}