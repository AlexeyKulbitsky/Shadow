#include "Material.h"
#include "MaterialParams.h"
#include "RenderTechnique.h"
#include "RenderPipeline.h"
#include "../Device.h"

#include "../font/FontManager.h"
#include "../font/Font.h"

#include <pugixml.hpp>

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

			while (paramNode)
			{
				String name = paramNode.name();
				// Read sampler
				if (name == "sampler")
				{
					SamplerDescription samplerDesc;

					String samplerName = paramNode.attribute("name").as_string();
					TexturePtr texture;

					pugi::xml_node typeNode = paramNode.child("type");
					pugi::xml_node textureNode = paramNode.child("texture");
					String typeName = typeNode.attribute("val").as_string();
					if (typeName == "Cube")
					{
						samplerDesc.type = GPOT_SAMPLER_CUBE;
						if (textureNode)
						{
							std::vector<String> faces(6);
							std::vector<String> facesNames = { "right", "left", "top", "bottom", "back", "front" };
							for (u32 i = 0; i < facesNames.size(); ++i)
							{
								String face = textureNode.child(facesNames[i].c_str()).attribute("val").as_string();
								faces[i] = face;
							}
							texture = resourceManager->GetCubeTexture(faces);
						}
						else
						{
							texture = resourceManager->GetDefaultTexture();
						}


					}
					else
					{
						samplerDesc.type = GPOT_SAMPLER_2D;
						if (textureNode)
						{
							String textureFilename = textureNode.attribute("val").as_string();
							texture = resourceManager->GetTexture(textureFilename);
						}
						else
						{
							texture = resourceManager->GetDefaultTexture();
						}

					}

					if (paramNode.child("minFilter"))
					{
						String val = paramNode.child("minFilter").attribute("val").as_string();
						if (textureFilteringMap.find(val) != textureFilteringMap.end())
							samplerDesc.minFilter = textureFilteringMap.at(val);
					}
					if (paramNode.child("magFilter"))
					{
						String val = paramNode.child("magFilter").attribute("val").as_string();
						if (textureFilteringMap.find(val) != textureFilteringMap.end())
							samplerDesc.magFilter = textureFilteringMap.at(val);
					}
					if (paramNode.child("mipFilter"))
					{
						String val = paramNode.child("mipFilter").attribute("val").as_string();
						if (textureFilteringMap.find(val) != textureFilteringMap.end())
							samplerDesc.mipFilter = textureFilteringMap.at(val);
					}
					if (paramNode.child("tilingU"))
					{
						String val = paramNode.child("tilingU").attribute("val").as_string();
						if (textureTilingMap.find(val) != textureTilingMap.end())
							samplerDesc.tilingU = textureTilingMap.at(val);
					}
					if (paramNode.child("tilingV"))
					{
						String val = paramNode.child("tilingV").attribute("val").as_string();
						if (textureTilingMap.find(val) != textureTilingMap.end())
							samplerDesc.tilingV = textureTilingMap.at(val);
					}
					if (paramNode.child("tilingW"))
					{
						String val = paramNode.child("tilingW").attribute("val").as_string();
						if (textureTilingMap.find(val) != textureTilingMap.end())
							samplerDesc.tilingV = textureTilingMap.at(val);
					}

					auto sampler = Sampler::Create(samplerDesc);
					sampler->Set(texture);
					//m_commonGpuParams->SetSampler(samplerName, texture);

					m_commonGpuParams->SetSampler(samplerName, sampler);
				}
				else if (name == "param")
				{
					String paramName = paramNode.attribute("name").as_string();
					String typeName = paramNode.child("type").attribute("val").as_string();
					auto it = materialParamTypeMap.find(typeName);
					if (it != materialParamTypeMap.end())
					{
						switch (it->second)
						{
							case MaterialParamType::Float:
							{
								float value = paramNode.child("value").attribute("val").as_float();
								m_commonGpuParams->SetParam(paramName, value);
							}
							break;
							case MaterialParamType::Float2:
							{
								math::Vector2f value;
								pugi::xml_node valueNode = paramNode.child("value");
								value.x = valueNode.attribute("x").as_float();
								value.y = valueNode.attribute("y").as_float();
								m_commonGpuParams->SetParam(paramName, value);
							}
							break;
							case MaterialParamType::Float3:
							{
								math::Vector3f value;
								pugi::xml_node valueNode = paramNode.child("value");
								value.x = valueNode.attribute("x").as_float();
								value.y = valueNode.attribute("y").as_float();
								value.z = valueNode.attribute("z").as_float();
								m_commonGpuParams->SetParam(paramName, value);
							}
							break;
							case MaterialParamType::Float4:
							{
								math::Vector4f value;
								pugi::xml_node valueNode = paramNode.child("value");
								value.x = valueNode.attribute("x").as_float();
								value.y = valueNode.attribute("y").as_float();
								value.z = valueNode.attribute("z").as_float();
								value.w = valueNode.attribute("w").as_float();
								m_commonGpuParams->SetParam(paramName, value);
							}
							break;
							case MaterialParamType::Color3:
							{
								math::Vector3f value;
								pugi::xml_node valueNode = paramNode.child("value");
								value.x = valueNode.attribute("x").as_float();
								value.y = valueNode.attribute("y").as_float();
								value.z = valueNode.attribute("z").as_float();
								m_commonGpuParams->SetParam(paramName, value);
							}
							break;
							case MaterialParamType::Color4:
							{
								math::Vector4f value;
								pugi::xml_node valueNode = paramNode.child("value");
								value.x = valueNode.attribute("x").as_float();
								value.y = valueNode.attribute("y").as_float();
								value.z = valueNode.attribute("z").as_float();
								value.w = valueNode.attribute("w").as_float();
								m_commonGpuParams->SetParam(paramName, value);
							}
							break;
							default:
								break;
						}
					}
				}

				paramNode = paramNode.next_sibling();
			}
		}

		////////////////////////////////////////////////////////////////

		void Material::Save()
		{
			if (m_fileInfo.expired())
				return;

			auto fileInfo = m_fileInfo.lock();
			// Save material to disk
			pugi::xml_document doc;
			pugi::xml_node materialNode = doc.append_child("material");
			Save(materialNode);
			const sh::String path = fileInfo->absolutePath;
			doc.save_file(path.c_str());
		}

		////////////////////////////////////////////////////////////////

		void Material::Save(pugi::xml_node &materialNode)
		{
			//pugi::xml_node materialNode = parent;// .append_child("material");
			materialNode.append_attribute("name").set_value(m_name.c_str());
			pugi::xml_node techniqueNode = materialNode.append_child("technique");
			techniqueNode.append_attribute("filename")
				.set_value(GetRenderTechnique()->GetFileInfo().lock()->name.c_str());

			pugi::xml_node paramsNode = materialNode.append_child("params");
			const auto& paramsInfo = m_commonGpuParams->GetParamsInfo();

			// Save params
			const auto& params = m_commonParams->GetParams();
			for (const auto& param : params)
			{
				if (param.GetUsage() != MaterialParamUsage::Undefined)
					continue;

				pugi::xml_node paramNode = paramsNode.append_child("param");
				paramNode.append_attribute("name").set_value(param.GetName().c_str());
				paramNode.append_child("type").append_attribute("val")
					.set_value(materialParamTypeStringArray[static_cast<size_t>( param.GetType() )].c_str());
				switch (param.GetType())
				{
					case MaterialParamType::Float:
					{
						float value = 0.0f;
						param.Get(value);
						pugi::xml_node valueNode = paramNode.append_child("value");
						valueNode.append_attribute("val").set_value(value);
					}
					break;
					case MaterialParamType::Float2:
					{
						math::Vector2f value;
						param.Get(value);
						pugi::xml_node valueNode = paramNode.append_child("value");
						valueNode.append_attribute("x").set_value(value.x);
						valueNode.append_attribute("y").set_value(value.y);
					}
					break;
					case MaterialParamType::Float3:
					{
						math::Vector3f value;
						param.Get(value);
						pugi::xml_node valueNode = paramNode.append_child("value");
						valueNode.append_attribute("x").set_value(value.x);
						valueNode.append_attribute("y").set_value(value.y);
						valueNode.append_attribute("z").set_value(value.z);
					}
					break;
					case MaterialParamType::Float4:
					{
						math::Vector4f value;
						param.Get(value);
						pugi::xml_node valueNode = paramNode.append_child("value");
						valueNode.append_attribute("x").set_value(value.x);
						valueNode.append_attribute("y").set_value(value.y);
						valueNode.append_attribute("z").set_value(value.z);
						valueNode.append_attribute("w").set_value(value.w);
					}
					break;
					case MaterialParamType::Color3:
					{
						math::Vector3f value;
						param.Get(value);
						pugi::xml_node valueNode = paramNode.append_child("value");
						valueNode.append_attribute("x").set_value(value.x);
						valueNode.append_attribute("y").set_value(value.y);
						valueNode.append_attribute("z").set_value(value.z);
					}
					break;
					case MaterialParamType::Color4:
					{
						math::Vector4f value;
						param.Get(value);
						pugi::xml_node valueNode = paramNode.append_child("value");
						valueNode.append_attribute("x").set_value(value.x);
						valueNode.append_attribute("y").set_value(value.y);
						valueNode.append_attribute("z").set_value(value.z);
						valueNode.append_attribute("w").set_value(value.w);
					}
					break;
					default:
						break;
				}
			}

			// Save samplers
			const auto& samplerParams = m_commonParams->GetSamplerParams();
			for (const auto& samplerParam : samplerParams)
			{
				const auto sampler = samplerParam.GetSampler();

				const auto& desc = sampler->GetDescription();
				pugi::xml_node samplerNode = paramsNode.append_child("sampler");

				samplerNode.append_attribute("name").set_value(samplerParam.GetName().c_str());
				samplerNode.append_child("type").append_attribute("val")
					.set_value(samplerTypeStringArray[desc.type].c_str());
				if (desc.type == GPOT_SAMPLER_3D || desc.type == GPOT_SAMPLER_CUBE)
				{
				}
				else
				{
					if (!sampler->GetTexture()->GetFileInfo().expired())
					{
						const auto& fileName = sampler->GetTexture()->GetFileInfo().lock()->name;

						samplerNode.append_child("texture").append_attribute("val")
							.set_value(fileName.c_str());
					}
				}

				samplerNode.append_child("minFilter").append_attribute("val")
					.set_value(textureFilteringStringArray[desc.minFilter].c_str());
				samplerNode.append_child("magFilter").append_attribute("val")
					.set_value(textureFilteringStringArray[desc.magFilter].c_str());
				samplerNode.append_child("mipFilter").append_attribute("val")
					.set_value(textureFilteringStringArray[desc.mipFilter].c_str());
				samplerNode.append_child("tilingU").append_attribute("val")
					.set_value(textureTilingStringArray[desc.tilingU].c_str());
				if (desc.type == GPOT_SAMPLER_2D)
				{
					samplerNode.append_child("tilingV").append_attribute("val")
						.set_value(textureTilingStringArray[desc.tilingV].c_str());
				}
				if (desc.type == GPOT_SAMPLER_3D || desc.type == GPOT_SAMPLER_CUBE)
				{
					samplerNode.append_child("tilingW").append_attribute("val")
						.set_value(textureTilingStringArray[desc.tilingW].c_str());
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
				m_commonParams.reset(new MaterialParams(m_commonGpuParams, technique->GetMaterialParamsDescription(i)));

				m_autoGpuParams = GpuParams::Create(pipeline->GetAutoParamsInfo());
				m_autoParams.reset(new MaterialParams(m_autoGpuParams, technique->GetMaterialParamsDescription(i)));
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