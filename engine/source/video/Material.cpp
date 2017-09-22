#include "Material.h"
#include "MaterialParams.h"
#include "RenderTechnique.h"
#include "RenderPipeline.h"
#include "../Device.h"

#include "../font/FontManager.h"
#include "../font/Font.h"

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
					TexturePtr texture;

					pugi::xml_attribute typeAttr = paramNode.attribute("type");
					if (typeAttr)
					{
						String typeName = typeAttr.as_string();
						if (typeName == "Cube")
						{
							std::vector<String> faces(6);
							std::vector<String> facesNames = { "right", "left", "top", "bottom", "back", "front" };
							for (u32 i = 0; i < facesNames.size(); ++i)
							{
								String face = paramNode.attribute(facesNames[i].c_str()).as_string();
								faces[i] = face;
							}

							texture = resourceManager->GetCubeTexture(faces);
						}
					}
					else
					{
						String textureFilename = paramNode.attribute("val").as_string();
						texture = resourceManager->GetTexture(textureFilename);
					}

					m_commonGpuParams->SetSampler(samplerName, texture);
				}

				paramNode = paramNode.next_sibling();
			}
		}

		////////////////////////////////////////////////////////////////

		void Material::Save(pugi::xml_node &materialNode)
		{
			//pugi::xml_node materialNode = parent;// .append_child("material");
			materialNode.append_attribute("name").set_value(m_name.c_str());
			pugi::xml_node techniqueNode = materialNode.append_child("technique");
			techniqueNode.append_attribute("filename").set_value(GetRenderTechnique()->GetFileName().c_str());


			const auto& paramsInfo = m_commonGpuParams->GetParamsInfo();
			for (size_t i = 0; i < 6; ++i)
			{
				const auto& paramsDescription = paramsInfo->GetParamsDescription(ShaderType(i));
				for (const auto& samplerDesc : paramsDescription->samplers)
				{
					const auto sampler = m_commonGpuParams->GetSampler(samplerDesc.second.name);
					const auto& fileName = sampler->GetTexture()->GetFileName();
				}
			}
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
				m_commonParams.reset(new MaterialParams(m_commonGpuParams));

				m_autoGpuParams = GpuParams::Create(pipeline->GetAutoParamsInfo());
				m_autoParams.reset(new MaterialParams(m_autoGpuParams));
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