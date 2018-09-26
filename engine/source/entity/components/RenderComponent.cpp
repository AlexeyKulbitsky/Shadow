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

	void RenderComponent::RegisterObject()
	{
		ObjectFactory::GetInstance()->RegisterFactory<RenderComponent>("Components");
		S_ACCESSOR_PROPERTY("Model", GetModelProperty, SetModelProperty);
		S_ACCESSOR_PROPERTY("Materials", GetMaterials, SetMaterials);
	}

	////////////////////////////////////////////////////////////////

	Component* RenderComponent::Clone()
	{
		RenderComponent* component = new RenderComponent();

		component->m_model = m_model;
		component->m_namedResourceRefList = m_namedResourceRefList;

		return component;
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
            m_namedResourceRefList[i].resource.type = "Materials";
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
            m_namedResourceRefList[i].resource.type = "Materials";
		}

		UpdateMaterials();
	}

	////////////////////////////////////////////////////////////////

	ResourceRef RenderComponent::GetModelProperty() const
	{
        if (m_model)
        {
            return ResourceRef("Models", m_model->GetFileInfo().lock()->name);
        }
        
		return ResourceRef("Models", "-None-");
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
