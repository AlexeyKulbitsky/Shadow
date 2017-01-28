#include "Driver.h"
#include "Material.h"
#include "RenderPipeline.h"
#include "DepthStencilState.h"
#include "RasterizationState.h"
#include "BlendingState.h"
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
				sh::scene::MeshPtr mesh = model->GetMesh(i);
				Render(mesh.get());
			}
		}

		/////////////////////////////////////////////////////////////////////////

		void Driver::Render(scene::Mesh* mesh)
		{
			Material* material = mesh->GetMaterial();
			RenderPipeline* renderPipeline = material->GetRenderPipeline(0);

			SetDepthStencilState(renderPipeline->GetDepthStencilState());
			SetRasterizationState(renderPipeline->GetRasterizationState());
			SetBlendingState(renderPipeline->GetBlendingState());
			renderPipeline->GetShaderProgram()->BindProgram();

			RenderCommand* renderCommand = mesh->GetRenderCommand();

			Render(renderCommand);
			renderPipeline->GetShaderProgram()->UnbindProgram();
		}

		/////////////////////////////////////////////////////////////////////////
		
		void Driver::SetViewport(u32 x, u32 y, u32 width, u32 height)
		{
			m_viewPort.x = x;
			m_viewPort.y = y;
			m_viewPort.z = width;
			m_viewPort.w = height;
		}

		/////////////////////////////////////////////////////////////////////////
	
		void Driver::SetDepthRange(f32 zMin, f32 zMax)
		{
			m_depthRange.x = zMin;
			m_depthRange.y = zMax;
		}

		/////////////////////////////////////////////////////////////////////////
	}
}