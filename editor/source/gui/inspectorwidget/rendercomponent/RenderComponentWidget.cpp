#include "RenderComponentWidget.h"

//#include "../../materialview/MaterialParamSamplerView.h"

MeshMaterialParam::MeshMaterialParam(const sh::scene::MeshPtr& mesh)
{
	m_mesh = mesh;
	const auto& material = m_mesh->GetMaterial();

	m_materialInfos = sh::io::FileSystem::GetInstance()->GetMaterialFileInfos();

	sh::gui::ComboBoxPtr comboBox(new sh::gui::ComboBox());

	sh::u32 index = 0U;
	for (sh::u32 i = 0U; i < m_materialInfos.size(); ++i)
	{
		if (!m_materialInfos[i].expired())
		{
			comboBox->AddItem(m_materialInfos[i].lock()->name);
			if (material->GetFileInfo().lock()->name == m_materialInfos[i].lock()->name)
				index = i;
		}
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
	const auto& name = m_materialInfos[index].lock()->name;
	
	sh::ResourceManager* resourceManager = sh::Device::GetInstance()->GetResourceManager();
	auto material = resourceManager->GetMaterial(name);

	auto rbManager = sh::video::RenderBatchManager::GetInstance();
	rbManager->RemoveMesh(m_mesh);

	m_mesh->SetMaterial(material);

	rbManager->AddMesh(m_mesh);
}


RenderComponentWidget::RenderComponentWidget()
	: ExpandableWidget("Render")
{
	m_modelFileInfos = sh::io::FileSystem::GetInstance()->GetModelFileInfos();

	m_modelComboBox.reset(new sh::gui::ComboBox());
	for (sh::u32 i = 0U; i < m_modelFileInfos.size(); ++i)
	{
		if (!m_modelFileInfos[i].expired())
		{
			m_modelComboBox->AddItem(m_modelFileInfos[i].lock()->name);
		}

	}
	m_modelComboBox->OnItemChanged.Connect(std::bind(&RenderComponentWidget::ModelChanged, this,
 		std::placeholders::_1));

	sh::gui::HorizontalLayoutPtr modelLayout(new sh::gui::HorizontalLayout());
	sh::gui::LabelPtr modelLabel(new sh::gui::Label("Model"));
	m_modelWidget.reset(new sh::gui::Widget());
	m_modelWidget->SetMaximumHeight(20);
	modelLayout->AddWidget(modelLabel);
	modelLayout->AddWidget(m_modelComboBox);
	m_modelWidget->SetLayout(modelLayout);
}

RenderComponentWidget::~RenderComponentWidget()
{

}

void RenderComponentWidget::SetRenderComponent(sh::RenderComponent* component)
{
	m_renderComponent = component;

	if (!component)
	{
		return;
	}
	m_contentsLayout->Clear();
	m_contentsLayout->AddWidget(m_modelWidget);

	const auto& modelName = component->GetModel()->GetFileInfo().lock()->name;

	for (size_t i = 0; i < m_modelFileInfos.size(); ++i)
	{
		if (m_modelFileInfos[i].lock()->name == modelName)
		{
			m_modelComboBox->SetSelectedItem(i);
			break;
		}
	}

	const auto& model = component->GetModel();
	size_t meshesCount = model->GetMeshesCount();
	for (size_t i = 0U; i < meshesCount; ++i)
	{
		const auto& mesh = model->GetMesh(i);
		sh::SPtr<MeshMaterialParam> param(new MeshMaterialParam(mesh));
		m_contentsLayout->AddWidget(param);
	}
}

void RenderComponentWidget::OnButtonToggled(bool toggled)
{

}

void RenderComponentWidget::ModelChanged(sh::u32 index)
{
	if (index >= m_modelFileInfos.size())
		return;

	if (!m_renderComponent)
		return;

	auto rbManager = sh::video::RenderBatchManager::GetInstance();
	auto resourceManager = sh::Device::GetInstance()->GetResourceManager();

	const auto& model = m_renderComponent->GetModel();
	if (model->GetFileInfo().lock()->name != m_modelFileInfos[index].lock()->name)
	{
		m_contentsLayout->Clear();
		m_contentsLayout->AddWidget(m_modelWidget);
		const auto modelMatrix = model->GetWorldMatrix();

		// Remove current model for rendering meshes
		const auto meshesCount = model->GetMeshesCount();
		for (size_t i = 0; i < meshesCount; ++i)
		{
			const auto& mesh = model->GetMesh(i);
			rbManager->RemoveMesh(mesh);
		}
		const sh::String modelName = m_modelFileInfos[index].lock()->name;
		auto modelBase = resourceManager->GetModelBase(modelName);
		
		sh::scene::ModelPtr newModel(new sh::scene::Model(modelBase));

		const auto& defaultMaterial = resourceManager->GetDefaultMaterial();
		const auto newModelMeshesCount = newModel->GetMeshesCount();
		for (size_t i = 0; i < newModelMeshesCount; ++i)
		{
			const auto& mesh = newModel->GetMesh(i);
			mesh->SetMaterial(defaultMaterial);
			rbManager->AddMesh(mesh);

			sh::SPtr<MeshMaterialParam> param(new MeshMaterialParam(mesh));
			m_contentsLayout->AddWidget(param);
		}

		auto box = newModel->GetInitialBoundingBox().GetTransformed(modelMatrix);
		newModel->SetBoundingBox(box);

		m_renderComponent->SetModel(newModel);
	}
}