#include "Driver.h"
#include "Material.h"
#include "RenderPass.h"
#include "RenderState.h"
#include "ShaderProgram.h"

#include "../scene/Mesh.h"
#include "../scene/Model.h"

namespace sh
{
	namespace video
	{
		template<typename T>
		void Driver::SetGlobalUniform(GlobalUniformName globalName, const T& value)
		{
			m_globalUniforms[(size_t)globalName]->Set(value);
		}

		/////////////////////////////////////////////////////////////////////////

		void Driver::InitGlobalUniforms()
		{
			m_globalUniforms[(size_t)GlobalUniformName::MODEL_WORLD_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::MODEL_NORMAL_MATRIX] = new Uniform(math::Matrix3f());

			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_VIEW_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_VIEW_TRANSLATION_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_VIEW_ROTATION_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_PROJECTION_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_VIEW_PROJECTION_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_VIEW_ROTATION_PROJECTION_MATRIX] = new Uniform(math::Matrix4f());						
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_POSITION] = new Uniform(math::Vector3f());

			// Light uniforms
			m_globalUniforms[(size_t)GlobalUniformName::LIGHT_DIRECTIONAL_LIGHTS_COUNT] = new Uniform(0);
			m_globalUniforms[(size_t)GlobalUniformName::LIGHT_DIRECTIONAL_DIRECTION] = new Uniform(std::vector<math::Vector3f>(0.0f));
			m_globalUniforms[(size_t)GlobalUniformName::LIGHT_DIRECTIONAL_COLOR] = new Uniform(std::vector<math::Vector3f>(0.0f));
		}

		/////////////////////////////////////////////////////////////////////////

		void Driver::Render(scene::Model* model)
		{
			size_t meshesCount = model->GetMeshesCount();
			for (size_t i = 0; i < meshesCount; ++i)
			{
				sh::scene::Mesh* mesh = model->GetMesh(i);

				Material* material = mesh->GetMaterial();
				RenderPass* renderPass = material->GetRenderPass(0);

				RenderState* renderState = renderPass->GetRenderState();
				renderState->Apply();
				renderPass->GetShaderProgram()->BindProgram();
		
				RenderCommand* renderCommand = mesh->GetRenderCommand();

				Render(renderCommand);
				renderPass->GetShaderProgram()->UnbindProgram();
		
			}
		}

		/////////////////////////////////////////////////////////////////////////
	}
}