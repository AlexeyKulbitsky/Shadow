#include "MaterialEditor.h"

#include "MaterialParamSamplerView.h"

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

MaterialEditor::MaterialEditor()
	: sh::gui::Window(sh::math::Recti(100, 100, 350, 500))
{
	SetText("Material Editor");

	m_layout.reset(new sh::gui::VerticalLayout());
	m_layout->SetMargins(2, 2, 2, 2);
	m_layout->SetSpacing(2);
	SetLayout(m_layout);
}

void MaterialEditor::SetMaterial(const sh::video::MaterialPtr& material)
{
	m_layout->Clear();
	m_material = material.get();
	if (!m_material)
		return;

	const auto& params = material->GetParams();
	sh::u32 paramsCount = params->GetParamsCount();
	for (sh::u32 i = 0; i < paramsCount; ++i)
	{
		auto param = params->GetParam(i);
		if (param->GetUsage() != sh::MaterialParamUsage::Undefined)
			continue;

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

	const auto& samplerParams = params->GetSamplerParams();
	for (const auto& samplerParam : samplerParams)
	{
		sh::gui::WidgetPtr widget;
		widget.reset(new MaterialParamSamplerView(const_cast<sh::video::MaterialSamplerParam*>(&samplerParam)));
		m_layout->AddWidget(widget);
	}
}