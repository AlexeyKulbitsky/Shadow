#include "MaterialWidget.h"

#include "materialview/MaterialParamSamplerView.h"

MeshMaterialParam::MeshMaterialParam(const sh::scene::MeshPtr& mesh)
{
	m_mesh = mesh.get();
	const auto& material = m_mesh->GetMaterial();

	m_materialNames = sh::io::FileSystem::GetInstance()->GetMaterialFileNames();

	sh::gui::ComboBoxPtr comboBox(new sh::gui::ComboBox());

	sh::u32 index = 0U;
	for (sh::u32 i = 0U; i < m_materialNames.size(); ++i)
	{
		comboBox->AddItem(m_materialNames[i]);
		if (material->GetFileName() == m_materialNames[i])
			index = i;
	}
	comboBox->SetSelectedItem(index);

	comboBox->OnItemChanged.Connect(std::bind(&MeshMaterialParam::MaterialChanged, this,
		std::placeholders::_1));

	sh::gui::HorizontalLayoutPtr layout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr label(new sh::gui::Label(mesh->GetName()));

	layout->AddWidget(label);
	layout->AddWidget(comboBox);
	SetLayout(layout);

	SetMaximumHeight(20U);
}

void MeshMaterialParam::MaterialChanged(sh::u32 index)
{
	const auto& name = m_materialNames[index];
	
	sh::ResourceManager* resourceManager = sh::Device::GetInstance()->GetResourceManager();
	auto material = resourceManager->GetMaterial(name);

	m_mesh->SetMaterial(material);
}


MaterialWidget::MaterialWidget()
{
	m_layout.reset(new sh::gui::VerticalLayout());
	m_layout->SetMargins(2, 2, 2, 2);
	m_layout->SetSpacing(2);

	m_widget.reset(new sh::gui::Widget());
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

	const auto& model = component->GetModel();
	size_t meshesCount = model->GetMeshesCount();
	for (size_t i = 0U; i < meshesCount; ++i)
	{
		const auto& mesh = model->GetMesh(i);
		sh::SPtr<MeshMaterialParam> param(new MeshMaterialParam(mesh));
		m_layout->AddWidget(param);
	}
}

void MaterialWidget::OnButtonToggled(bool toggled)
{

}