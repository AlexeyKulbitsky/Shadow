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
#include "../../video/UniformBuffer.h"
#include "../../scene/SceneManager.h"


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

		// Read material
		pugi::xml_node materialNode = node.child("material");
		if (materialNode)
		{
			pugi::xml_attribute nameAttribute = materialNode.attribute("name");
			SH_ASSERT(nameAttribute);

			String materialName(nameAttribute.as_string());

			const sh::video::MaterialPtr& material = resourceManager->GetMaterial(materialName);
			SH_ASSERT(material);
				
			// Init model with material
			for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
			{
				sh::scene::MeshPtr mesh = model->GetMesh(i);
				mesh->SetMaterial(material);
			}		

			// Reinit samplers if base mesh has materials with model
			{
				for (size_t i = 0; i < modelBase->GetMeshesCount(); ++i)
				{
					scene::MeshBasePtr meshBase = modelBase->GetMesh(i);
					scene::MeshPtr mesh = model->GetMesh(i);
					const video::UniformBufferPtr& meshUniformBuffer = mesh->GetMaterial()->GetRenderPipeline()->GetUniformBuffer();

					for (size_t j = 0; j < meshBase->GetSamplersCount(); ++j)
					{
						video::Sampler* baseSampler = meshBase->GetSampler(j);
						video::Sampler* sampler = meshUniformBuffer->GetSampler(baseSampler->GetUsage());
						if (sampler)
						{
							sh::video::TexturePtr texture = resourceManager->GetTexture(baseSampler->GetTextureName());
							sampler->Set(texture);
						}
					}
				}
			}

			m_model = model;
		}
	}

	////////////////////////////////////////////////////////////////

	void RenderComponent::Save(pugi::xml_node &parent)
	{
		pugi::xml_node componentNode = parent.append_child("component");
		componentNode.append_attribute("name").set_value("render");

		// Save model
		pugi::xml_node modelNode = componentNode.append_child("model");
		modelNode.append_attribute("filename").set_value(m_model->GetFileName().c_str());
		
		// Save material
		const sh::video::MaterialPtr& material = m_model->GetMesh(0)->GetMaterial();
		pugi::xml_node materialNode = componentNode.append_child("material");
		materialNode.append_attribute("name").set_value(material->GetName().c_str());
	}

	////////////////////////////////////////////////////////////////

}