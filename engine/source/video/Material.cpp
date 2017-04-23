#include "Material.h"
#include "RenderTechnique.h"
#include "RenderPipeline.h"
#include "../Device.h"

namespace sh
{
	namespace video
	{
		void Material::Load(const pugi::xml_node &node)
		{
			pugi::xml_attribute nameAttribute = node.attribute("name");
			if (nameAttribute)
			{
				m_name = nameAttribute.as_string();
			}

			pugi::xml_node techniqueNode = node.child("technique");
			pugi::xml_attribute fileName = techniqueNode.attribute("filename");
			String techniqueFileName = fileName.as_string();

			ResourceManager* resourceManager = Device::GetInstance()->GetResourceManager();

			sh::video::RenderTechniquePtr rt = resourceManager->GetRenderTechnique(techniqueFileName);

			SetRenderTechnique(rt);

			pugi::xml_node paramsNode = node.child("params");

			pugi::xml_node paramNode = paramsNode.first_child();

			while( paramNode )
			{
				String name = paramNode.name();
				if( name == "sampler" )
				{
					String samplerName = paramNode.attribute("name").as_string();
					String textureFilename = paramNode.attribute("val").as_string();

					TexturePtr texture = resourceManager->GetTexture(textureFilename);

					m_commonGpuParams->SetSampler(ST_FRAGMENT, samplerName, texture);
				}

				paramNode = paramNode.next_sibling();
			}

			// Read uniforms

			// Read samplers
			/*
			pugi::xml_node samplersNode = node.child("samplers");
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
			*/
		}

		////////////////////////////////////////////////////////////////

		void Material::Save(pugi::xml_node &parent)
		{
			pugi::xml_node materialNode = parent.append_child("material");
			materialNode.append_attribute("name").set_value(m_name.c_str());
			pugi::xml_node techniqueNode = materialNode.append_child("technique");
			techniqueNode.append_attribute("filename").set_value(GetRenderTechnique()->GetFileName().c_str());
		}

		////////////////////////////////////////////////////////////////

		void Material::SetRenderTechnique(const RenderTechniquePtr& technique) 
		{ 
			m_renderTechnique = technique; 
			size_t pipelinesCount = technique->GetRenderPipelinesCount();
			if (m_renderPipelines.size() != pipelinesCount)
			{
				m_renderPipelines.resize(pipelinesCount);
			}

			for (size_t i = 0; i < pipelinesCount; ++i)
			{			
				RenderPipelinePtr pipeline = technique->GetRenderPipeline(i);
				m_renderPipelines[i] = pipeline;
				m_commonGpuParams = GpuParams::Create(pipeline->GetParamsInfo());
			}
		}

		////////////////////////////////////////////////////////////////

		void Material::SetRenderTechnique(const String& techniqueName)
		{
			ResourceManager* resourceManager = Device::GetInstance()->GetResourceManager();
			sh::video::RenderTechniquePtr rt = resourceManager->GetRenderTechnique(techniqueName);

			SetRenderTechnique(rt);
		}

		////////////////////////////////////////////////////////////////
	}
}