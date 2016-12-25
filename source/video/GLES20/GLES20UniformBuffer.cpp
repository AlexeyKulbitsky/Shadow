#include "GLES20UniformBuffer.h"
#include "GLES20ShaderProgram.h"
#include "GLES20Uniform.h"
#include "GLES20Sampler.h"

namespace sh
{
	namespace video
	{
		void GLES20UniformBuffer::Load(const pugi::xml_node &node)
		{
			// Load uniforms
			pugi::xml_node uniformsNode = node.child("uniforms");
			LoadUniforms(uniformsNode);

			// Load texure samplers
			pugi::xml_node samplersNode = node.child("samplers");
			LoadSamplers(samplersNode);
		}

		/////////////////////////////////////////////////////////////////////////

		void GLES20UniformBuffer::Init()
		{
			m_shaderProgram->BindProgram();
			UniformBuffer::Init();
			m_shaderProgram->UnbindProgram();
		}

		/////////////////////////////////////////////////////////////////////////

		void GLES20UniformBuffer::SetShaderProgram(ShaderProgram* shaderProgram)
		{
			m_shaderProgram = static_cast<GLES20ShaderProgram*>(shaderProgram);
		}

		/////////////////////////////////////////////////////////////////////////

		void GLES20UniformBuffer::LoadUniforms(const pugi::xml_node &node)
		{
			if (node.empty())
				return;

			Uniform* uniform = nullptr;

			pugi::xml_node uniformNode = node.first_child();
			while (!uniformNode.empty())
			{
				pugi::xml_attribute nameAttr = uniformNode.attribute("name");
				pugi::xml_attribute typeAttr = uniformNode.attribute("type");
				pugi::xml_attribute valAttr = uniformNode.attribute("val");

				printf("Uniform:\n");
				std::string name = nameAttr.as_string();
				printf("\tName: %s ", name.c_str());

				std::string typeName = typeAttr.as_string();
				printf("Type: %s ", typeName.c_str());
				if (typeName == "float")
				{
					GLES20UniformFloat* glesUniform = new GLES20UniformFloat(0.0f);
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{
						float value = valAttr.as_float();
						printf("Value: %f", value);
						uniform->Set(value);
					}

				}
				else if (typeName == "int")
				{
					GLES20UniformInt *glesUniform = new GLES20UniformInt(0);
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{
						int value = valAttr.as_int();
						printf("Value: %d", value);
						uniform->Set(value);
					}
				}
				else if (typeName == "vec2")
				{
					GLES20UniformVector2f *glesUniform = new GLES20UniformVector2f(sh::math::Vector2f(0.0f));
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{

					}
				}
				else if (typeName == "vec3")
				{
					GLES20UniformVector3f *glesUniform = new GLES20UniformVector3f(sh::math::Vector3f(0.0f));
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{									
					}
				}
				else if (typeName == "vec4")
				{
					GLES20UniformVector4f *glesUniform = new GLES20UniformVector4f(sh::math::Vector4f(0.0f));
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{

					}
				}
				else if (typeName == "mat4")
				{
					GLES20UniformMatrix4f *glesUniform = new GLES20UniformMatrix4f(sh::math::Matrix4f());
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					if (valAttr)
					{
						String value = valAttr.as_string();
						if (value == "model.worldMatrix")
						{
							glesUniform->SetGlobalUniformName(GlobalUniformName::MODEL_WORLD_MATRIX);
						}
						else if (value == "model.worldViewProjectionMatrix")
						{
							glesUniform->SetGlobalUniformName(GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX);
						}
						else if (value == "camera.viewProjectionMatrix")
						{
							glesUniform->SetGlobalUniformName(GlobalUniformName::CAMERA_VIEW_PROJECTION_MATRIX);
						}
						else if (value == "camera.viewRotationProjectionMatrix")
						{
							glesUniform->SetGlobalUniformName(GlobalUniformName::CAMERA_VIEW_ROTATION_PROJECTION_MATRIX);
						}
					}
				}
				else if (typeName == "vec3Array")
				{
					GLES20UniformVector3fArray* glesUniform = new GLES20UniformVector3fArray(std::vector<math::Vector3f>(0.0f));
					glesUniform->SetGLId(m_shaderProgram->GetGLId());
					uniform = glesUniform;
					String value = valAttr.as_string();
					if (value == "light.directional.direction")
					{
						glesUniform->SetGlobalUniformName(GlobalUniformName::LIGHT_DIRECTIONAL_DIRECTION);
					}
				}

				printf("\n");

				uniform->SetName(name);

				AddUniform(uniform);
				uniform = nullptr;

				uniformNode = uniformNode.next_sibling();
			}
		}

		/////////////////////////////////////////////////////////////////////////

		void GLES20UniformBuffer::LoadSamplers(const pugi::xml_node &node)
		{
			if (node.empty())
				return;

			pugi::xml_node uniformNode = node.first_child();

			GLES20Sampler* sampler = new GLES20Sampler(m_shaderProgram->GetGLId());

			while (!uniformNode.empty())
			{
				pugi::xml_attribute nameAttr = uniformNode.attribute("name");
				pugi::xml_attribute typeAttr = uniformNode.attribute("type");
				pugi::xml_attribute tilingUAttr = uniformNode.attribute("tilingU");
				pugi::xml_attribute tilingVAttr = uniformNode.attribute("tilingV");
				pugi::xml_attribute tilingWAttr = uniformNode.attribute("tilingW");
				pugi::xml_attribute filerAttr = uniformNode.attribute("filter");
				pugi::xml_attribute usageAttr = uniformNode.attribute("usage");

				printf("Sampler:\n");
				std::string name = nameAttr.as_string();
				printf("\tName: %s ", name.c_str());

				// Read texture type
				std::string typeName = typeAttr.as_string();
				printf("Type: %s ", typeName.c_str());
				if (typeName == "2D")
				{
					sampler->SetType(Texture::Type::TEXTURE_2D);
				}
				else if (typeName == "cube")
				{
					sampler->SetType(Texture::Type::TEXTURE_CUBE);
				}


				// Read tiling
				Texture::Tiling tilingU = Texture::Tiling::REPEAT;
				Texture::Tiling tilingV = Texture::Tiling::REPEAT;
				Texture::Tiling tilingW = Texture::Tiling::REPEAT;

				if (tilingUAttr)
				{
					std::string tilingTypeName = tilingUAttr.as_string();
					if (tilingTypeName == "clamp")
					{
						tilingU = Texture::Tiling::CLAMP_TO_EDGE;
					}
				}

				if (tilingVAttr)
				{
					std::string tilingTypeName = tilingVAttr.as_string();
					if (tilingTypeName == "clamp")
					{
						tilingV = Texture::Tiling::CLAMP_TO_EDGE;
					}
				}
				if (tilingWAttr)
				{
					std::string tilingTypeName = tilingVAttr.as_string();
					if (tilingTypeName == "clamp")
					{
						tilingW = Texture::Tiling::CLAMP_TO_EDGE;
					}
				}


				// Read filter
				Texture::Filtering filtering = Texture::Filtering::NEAREST;
				if (filerAttr)
				{
					std::string filterTypeName = filerAttr.as_string();
					if (filterTypeName == "nearest")
					{
						filtering = Texture::Filtering::NEAREST;
					}
					else if (filterTypeName == "linear")
					{
						filtering = Texture::Filtering::LINEAR;
					}
					else if (filterTypeName == "bilinear")
					{
						filtering = Texture::Filtering::BILINEAR;
					}
					else if (filterTypeName == "trilinear")
					{
						filtering = Texture::Filtering::TRILINEAR;
					}
				}

				if (!usageAttr.empty())
				{
					String usage = usageAttr.as_string();
					if (usage == "diffuse_map")
					{
						sampler->SetUsage(Sampler::Usage::DIFFUSE_MAP);
					}
				}

				printf("\n");

				sampler->SetName(name);
				sampler->SetTiling(tilingU, tilingV, tilingW);
				sampler->SetFiltering(filtering);
				sampler->Init();

				AddSampler(sampler);

				uniformNode = uniformNode.next_sibling();
			}
		}

		/////////////////////////////////////////////////////////////////////////
	}
}