#include "Material.h"
#include "RenderTechnique.h"
#include "RenderPipeline.h"
#include "UniformBuffer.h"
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

			// Read uniforms
			const sh::video::UniformBufferPtr& uniformBuffer = GetRenderPipeline(0)->GetUniformBuffer();
			pugi::xml_node uniformsNode = node.child("uniforms");
			
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
			}

			// Split params description

			/*
			GpuParamsDescription desc = m_renderPipelines[0]->GetGpuParamsDescription();
			GpuParamsDescription transformDesc;
			for (auto it = desc.params.begin(); it != desc.params.end();)
			{
				if (it->first == "matWVP" ||
					it->first == "matWorld" ||
					it->first == "matView" ||
					it->first == "matProjection")
				{
					transformDesc.params[it->first] = it->second;
					it = desc.params.erase(it);
				}
				else
				{
					it++;
				}
			}

			m_commonGpuParams = GpuParams::Create(desc);
			m_transfromsGpuParams = GpuParams::Create(transformDesc);

			const u8* dataPtr = m_transfromsGpuParams->GetData();
			for (const auto& param : transformDesc.params)
			{
				const u8* data = dataPtr + param.second.offset;

				MaterialParamType type = MaterialParamType::Undefined;
				if (param.first == "matWVP")
					type = MaterialParamType::MatrixWorldViewProjection;

				SPtr<MaterialParam> matParam(new MaterialParam(type, const_cast<u8*>(data)));
				m_transformParams.push_back(matParam);
			}
			*/
		}
	}
}