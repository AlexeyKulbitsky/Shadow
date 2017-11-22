#include "RenderComponent.h"
#include "../../Device.h"
#include "../../resources/ResourceManager.h"
#include "../../scene/ModelBase.h"
#include "../../scene/MeshBase.h"
#include "../../scene/Model.h"
#include "../../scene/Mesh.h"
#include "../../video/Material.h"
#include "../../video/RenderTechnique.h"
#include "../../video/RenderPipeline.h"
#include "../../scene/SceneManager.h"
#include "../../serialization/ObjectFactory.h"


namespace sh
{
	RenderComponent::RenderComponent()
	{

	}

	////////////////////////////////////////////////////////////////

	RenderComponent::~RenderComponent()
	{

	}

	////////////////////////////////////////////////////////////////

	void RenderComponent::Load(const pugi::xml_node &node)
	{
		ResourceManager* resourceManager = Device::GetInstance()->GetResourceManager();

		// Read model
		sh::scene::ModelPtr model = nullptr;
		sh::scene::ModelBasePtr modelBase = nullptr;
		pugi::xml_node modelNode = node.child("model");
		if (modelNode)
		{
			pugi::xml_attribute attr = modelNode.attribute("filename");
			String modelFileName = attr.as_string();
			modelBase = resourceManager->GetModelBase(modelFileName);
			model.reset(new scene::Model(modelBase));
		}
		const auto& defaultMaterial = resourceManager->GetDefaultMaterial();

		pugi::xml_node materialsNode = node.child("materials");
		if (materialsNode)
		{
			// Global material for all meshes
			pugi::xml_attribute globalAttribute = materialsNode.attribute("global");
			if (globalAttribute)
			{
				String materialName(globalAttribute.as_string());
				const sh::video::MaterialPtr& material = resourceManager->GetMaterial(materialName);

				for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
				{
					sh::scene::MeshPtr mesh = model->GetMesh(i);
					mesh->SetMaterial(material);
				}
			}
			else
			{
				// Separate materials for each mesh
				pugi::xml_node materialNode = materialsNode.child("material");
				while (materialNode)
				{
					pugi::xml_attribute meshAttribute = materialNode.attribute("mesh");
					String meshName(meshAttribute.as_string());

					pugi::xml_attribute nameAttribute = materialNode.attribute("filename");
					String materialName(nameAttribute.as_string());

					const sh::video::MaterialPtr& material = resourceManager->GetMaterial(materialName);

					for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
					{
						sh::scene::MeshPtr mesh = model->GetMesh(i);
						if (mesh->GetName() == meshName)
						{
							mesh->SetMaterial(material);
						}
					}
					materialNode = materialNode.next_sibling();
				}
			}

			// Check all meshes for materials
			for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
			{
				sh::scene::MeshPtr mesh = model->GetMesh(i);
				if (!mesh->GetMaterial())
					mesh->SetMaterial(defaultMaterial);
			}
		}
		else
		{
			// Init model with default material
			for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
			{
				const auto& mesh = model->GetMesh(i);
				mesh->SetMaterial(defaultMaterial);
			}
		}

		m_model = model;

		/////////////////////////////////////////////////////////////

		for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
		{
			const auto& mesh = model->GetMesh(i);
			const auto& material = mesh->GetMaterial();

			NamedResourceRef ref;
			ref.name = mesh->GetName();
			ref.resource.type = "Materials";
			ref.resource.name = material->GetFileInfo().lock()->name;

			m_namedResourceRefList.push_back(ref);
		}
	}

	////////////////////////////////////////////////////////////////

	void RenderComponent::Save(pugi::xml_node &parent)
	{
		pugi::xml_node componentNode = parent.append_child("component");
		componentNode.append_attribute("name").set_value("render");

		// Save model
		pugi::xml_node modelNode = componentNode.append_child("model");
		modelNode.append_attribute("filename").set_value(m_model->GetFileInfo().lock()->name.c_str());
		
		// Save material
		pugi::xml_node materialsNode = componentNode.append_child("materials");
		const auto meshesCount = m_model->GetMeshesCount();
		for (size_t i = 0U; i < meshesCount; ++i)
		{

			pugi::xml_node materialNode = materialsNode.append_child("material");

			const auto& mesh = m_model->GetMesh(i);
			pugi::xml_attribute meshAttribute = materialNode.append_attribute("mesh");
			meshAttribute.set_value(mesh->GetName().c_str());

			pugi::xml_attribute nameAttribute = materialNode.append_attribute("filename");
			nameAttribute.set_value(mesh->GetMaterial()->GetFileInfo().lock()->name.c_str());
		}
	}

	////////////////////////////////////////////////////////////////

	void RenderComponent::RegisterObject()
	{
		ObjectFactory::GetInstance()->RegisterFactory<RenderComponent>();
		S_ACCESSOR_PROPERTY("Model", GetModelProperty, SetModelProperty);
		S_ACCESSOR_PROPERTY("Materials", GetMaterials, SetMaterials);
	}

	////////////////////////////////////////////////////////////////

	void RenderComponent::SetModel(scene::ModelPtr model) 
	{
		m_model = model; 

		if (!m_model)
			return;

		if (m_namedResourceRefList.size() != m_model->GetMeshesCount())
			m_namedResourceRefList.resize(m_model->GetMeshesCount());
		for (size_t i = 0, sz = m_model->GetMeshesCount(); i < sz; ++i)
		{
			m_namedResourceRefList[i].name = m_model->GetMesh(i)->GetName();
		}

		UpdateMaterials();
	}

	////////////////////////////////////////////////////////////////

	const scene::ModelPtr& RenderComponent::GetModel() 
	{ 
		return m_model; 
	}

	////////////////////////////////////////////////////////////////

	void RenderComponent::SetModelProperty(const ResourceRef& value)
	{
		auto modelBase = ResourceManager::GetInstance()->GetModelBase(value.name);
		m_model.reset(new scene::Model(modelBase));

		if (m_namedResourceRefList.size() != m_model->GetMeshesCount())
			m_namedResourceRefList.resize(m_model->GetMeshesCount());
		for (size_t i = 0, sz = m_model->GetMeshesCount(); i < sz; ++i)
		{
			m_namedResourceRefList[i].name = m_model->GetMesh(i)->GetName();
		}

		UpdateMaterials();
	}

	////////////////////////////////////////////////////////////////

	ResourceRef RenderComponent::GetModelProperty() const
	{
		return ResourceRef("Models", m_model->GetFileInfo().lock()->name);
	}

	////////////////////////////////////////////////////////////////

	void RenderComponent::SetMaterials(const NamedResourceRefList& value) 
	{ 
		m_namedResourceRefList = value; 

		if (!m_model)
			return;

		UpdateMaterials();
	}

	////////////////////////////////////////////////////////////////

	const NamedResourceRefList& RenderComponent::GetMaterials() const 
	{ 
		return m_namedResourceRefList; 
	}

	////////////////////////////////////////////////////////////////

	void RenderComponent::UpdateMaterials()
	{
		for (size_t i = 0, sz = m_model->GetMeshesCount(); i < sz; ++i)
		{
			const auto& mesh = m_model->GetMesh(i);
			video::MaterialPtr material;
			if (i < m_namedResourceRefList.size())
			{
				material = ResourceManager::GetInstance()->GetMaterial(m_namedResourceRefList[i].resource.name);
				if (!material)
					material = ResourceManager::GetInstance()->GetDefaultMaterial();
			}
			else
				material = ResourceManager::GetInstance()->GetDefaultMaterial();
			mesh->SetMaterial(material);
		}
	}

	////////////////////////////////////////////////////////////////
}