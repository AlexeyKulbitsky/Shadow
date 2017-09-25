#include "MaterialEditor.h"

#include "MaterialParamSamplerView.h"

MaterialParamFloatEditor::MaterialParamFloatEditor(sh::video::MaterialParam* param) : m_param(param)
{
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(m_param->GetName()));
	label->SetMaximumWidth(75U);
	layout->AddWidget(label);
	sh::gui::FloatLineEditPtr editWidget(new sh::gui::FloatLineEdit());
	layout->AddWidget(editWidget);
	SetLayout(layout);
	SetMaximumHeight(20);

	float value = 0.0f;
	m_param->Get(value);
	editWidget->SetValue(value);

	editWidget->OnValueChanged.Connect(
		std::bind(&MaterialParamFloatEditor::SetValue, this, std::placeholders::_1));
}

void MaterialParamFloatEditor::SetValue(float value)
{
	if (m_param)
		m_param->Set(value);
}

///////////////////////////////////////////////////////////////////////////////////

MaterialParamVector2Editor::MaterialParamVector2Editor(sh::video::MaterialParam* param) : m_param(param)
{
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(m_param->GetName()));
	label->SetMaximumWidth(75U);
	layout->AddWidget(label);
	sh::SPtr<Vector2LineEdit> editWidget(new Vector2LineEdit());
	layout->AddWidget(editWidget);
	SetLayout(layout);
	SetMaximumHeight(20);

	sh::math::Vector2f value;
	m_param->Get(value);
	editWidget->SetValue(value);

	editWidget->valueChanged.Connect(
		std::bind(&MaterialParamVector2Editor::SetValue, this, std::placeholders::_1));
}

void MaterialParamVector2Editor::SetValue(const sh::math::Vector2f& value)
{
	if (m_param)
		m_param->Set(value);
}

///////////////////////////////////////////////////////////////////////////////////

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

MaterialParamVector4Editor::MaterialParamVector4Editor(sh::video::MaterialParam* param) : m_param(param)
{
	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(m_param->GetName()));
	label->SetMaximumWidth(75U);
	layout->AddWidget(label);
	sh::SPtr<Vector4LineEdit> editWidget(new Vector4LineEdit());
	layout->AddWidget(editWidget);
	SetLayout(layout);
	SetMaximumHeight(20);

	sh::math::Vector4f value;
	m_param->Get(value);
	editWidget->SetValue(value);

	editWidget->valueChanged.Connect(
		std::bind(&MaterialParamVector4Editor::SetValue, this, std::placeholders::_1));
}

void MaterialParamVector4Editor::SetValue(const sh::math::Vector4f& value)
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

	m_rtInfos = sh::io::FileSystem::GetInstance()->GetRenderTechniqueFileInfos();

	sh::gui::HorizontalLayoutPtr rtLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr rtLabel(new sh::gui::Label("Render technique"));
	m_comboBox.reset(new sh::gui::ComboBox());

	sh::u32 index = 0U;
	for (sh::u32 i = 0U; i < m_rtInfos.size(); ++i)
	{
		m_comboBox->AddItem(m_rtInfos[i].lock()->name);
	}

	m_comboBox->OnItemChanged.Connect(std::bind(&MaterialEditor::OnRenderTechniqueChanged, this,
		std::placeholders::_1));

	rtLayout->AddWidget(rtLabel);
	rtLayout->AddWidget(m_comboBox);
	m_rtWidget.reset(new sh::gui::Widget());
	m_rtWidget->SetMaximumHeight(20U);
	m_rtWidget->SetLayout(rtLayout);
	m_layout->AddWidget(m_rtWidget);

	SetLayout(m_layout);
}

void MaterialEditor::SetMaterial(const sh::video::MaterialPtr& material)
{
	if (m_material == material.get())
		return;

	// Save current edited material
	if (m_material && m_material->HasChanged())
	{
		m_material->SetChanged(false);
		m_material->Save();
	}

	m_material = material.get();

	if (m_material)
	{
		sh::u32 index = 0U;
		for (sh::u32 i = 0U; i < m_rtInfos.size(); ++i)
		{
			if (m_material->GetRenderTechnique()->GetFileName() == m_rtInfos[i].lock()->name)
				index = i;
		}
		m_comboBox->SetSelectedItem(index);
	}

	ResetLayout();
}

void MaterialEditor::OnRenderTechniqueChanged(sh::u32 index)
{
	const auto& name = m_rtInfos[index].lock()->name;

	if (!m_material)
		return;

	if (m_material->GetRenderTechnique()->GetFileName() == name)
		return;

	// Check for textures availability
	m_material->SetRenderTechnique(name);
	ResetLayout();
}

void MaterialEditor::ResetLayout()
{
	m_layout->Clear();
	if (!m_material)
		return;

	m_layout->AddWidget(m_rtWidget);

	////////////////////////////////////////////////////////////////////////////////

	const auto& params = m_material->GetParams();
	sh::u32 paramsCount = params->GetParamsCount();
	for (sh::u32 i = 0; i < paramsCount; ++i)
	{
		auto param = params->GetParam(i);
		if (param->GetUsage() != sh::MaterialParamUsage::Undefined)
			continue;

		sh::gui::WidgetPtr widget;
		switch (param->GetType())
		{
		case sh::MaterialParamType::Float:
			widget.reset(new MaterialParamFloatEditor(param));
			break;
		case sh::MaterialParamType::Float2:
			widget.reset(new MaterialParamVector2Editor(param));
			break;
		case sh::MaterialParamType::Float3:
			widget.reset(new MaterialParamVector3Editor(param));
			break;
		case sh::MaterialParamType::Float4:
			widget.reset(new MaterialParamVector4Editor(param));
			break;
		default:
			break;
		}
		if (widget)
			m_layout->AddWidget(widget);
	}

	const auto& samplerParams = params->GetSamplerParams();
	for (const auto& samplerParam : samplerParams)
	{
		sh::SPtr<MaterialParamSamplerView> widget;
		widget.reset(new MaterialParamSamplerView(const_cast<sh::video::MaterialSamplerParam*>(&samplerParam)));
		widget->paramChanged.Connect(std::bind(&MaterialEditor::MaterialChanged, this));
		m_layout->AddWidget(widget);
	}
}

void MaterialEditor::MaterialChanged()
{
	if (m_material)
		m_material->SetChanged(true);
}