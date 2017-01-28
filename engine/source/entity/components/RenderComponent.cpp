#include "RenderComponent.h"
#include "../../Device.h"
#include "../../resources/ResourceManager.h"
#include "../../scene/ModelBase.h"
#include "../../scene/MeshBase.h"
#include "../../scene/Model.h"
#include "../../scene/Mesh.h"
#include "../../video/Material.h"
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
		sh::scene::Model* model = nullptr;
		sh::scene::ModelBasePtr modelBase = nullptr;
		pugi::xml_node modelNode = node.child("model");
		if (modelNode)
		{
			pugi::xml_attribute attr = modelNode.attribute("filename");
			String modelFileName = attr.as_string();
			modelBase = resourceManager->GetModelBase(modelFileName);
			model = new scene::Model(modelBase);
		}

		// Read material
		pugi::xml_node materialNode = node.child("material");
		if (materialNode)
		{
			sh::video::Material* material = new sh::video::Material();

			pugi::xml_node techniqueNode = materialNode.child("technique");
			pugi::xml_attribute fileName = techniqueNode.attribute("filename");
			String techniqueFileName = fileName.as_string();

			sh::video::RenderTechniquePtr rt = resourceManager->GetRenderTechnique(techniqueFileName);

			material->SetRenderTechnique(rt);

			// Read uniforms
			sh::video::UniformBuffer* uniformBuffer = material->GetRenderPipeline(0)->GetUniformBuffer();

			pugi::xml_node uniformsNode = materialNode.child("uniforms");
			if (uniformsNode)
			{
				pugi::xml_node uniNode = uniformsNode.first_child();
						
				while (!uniNode.empty())
				{
					pugi::xml_attribute nameAttr = uniNode.attribute("name");
					pugi::xml_attribute typeAttr = uniNode.attribute("type");
					pugi::xml_attribute valAttr = uniNode.attribute("val");

					std::string name = nameAttr.as_string();
					std::string typeName = typeAttr.as_string();

							
					sh::video::Uniform* uniform = uniformBuffer->GetUniform(name);
					if (typeName == "float")
					{
						float value = valAttr.as_float();							
						uniform->Set(value);

					}
					else if (typeName == "int")
					{
						int value = valAttr.as_int();
						uniform->Set(value);
					}
					uniNode = uniNode.next_sibling();
				}
			}

			// Read samplers
			pugi::xml_node samplersNode = materialNode.child("samplers");
			if (samplersNode)
			{
				pugi::xml_node samplNode = samplersNode.first_child();

				while (!samplNode.empty())
				{
					pugi::xml_attribute typeAttr = samplNode.attribute("type");
					String typeName = typeAttr.as_string();

					pugi::xml_attribute nameAttr = samplNode.attribute("name");
					std::string name = nameAttr.as_string();

					sh::video::Sampler* sampler = uniformBuffer->GetSampler(name);
					sh::video::TexturePtr texture = nullptr;
					if (typeName == "2D")
					{
						pugi::xml_attribute fileNameAttr = samplNode.attribute("filename");
						std::string fileName = fileNameAttr.as_string();
						texture = resourceManager->GetTexture(fileName);
					}
					else if (typeName == "cube")
					{
						pugi::xml_attribute right = samplNode.attribute("right");
						pugi::xml_attribute left = samplNode.attribute("left");
						pugi::xml_attribute top = samplNode.attribute("top");
						pugi::xml_attribute bottom = samplNode.attribute("bottom");
						pugi::xml_attribute back = samplNode.attribute("back");
						pugi::xml_attribute front = samplNode.attribute("front");

						std::vector<String> faces;
						faces.push_back(right.as_string());
						faces.push_back(left.as_string());
						faces.push_back(top.as_string());
						faces.push_back(bottom.as_string());
						faces.push_back(back.as_string());
						faces.push_back(front.as_string());

						texture = resourceManager->GetCubeTexture(faces);
					}
													

					sampler->Set(texture);

					samplNode = samplNode.next_sibling();
				}
			}

					
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
					video::UniformBuffer* meshUniformBuffer = mesh->GetRenderCommand()->GetUniformBuffer();

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
			Device::GetInstance()->GetSceneManager()->AddModel(m_model);
		}
	}

	////////////////////////////////////////////////////////////////

}