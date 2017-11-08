#include "Picker.h"
#include "../Device.h"
#include "../video/Driver.h"
#include "../video/Material.h"
#include "../video/RenderPipeline.h"
#include "../video/Renderable.h"
#include "../video/VertexDeclaration.h"
#include "../video/VertexBuffer.h"
#include "../scene/Mesh.h"
#include "../scene/Model.h"
#include "../scene/Camera.h"
#include "../scene/SceneManager.h"
#include "../entity/Entity.h"
#include "../entity/Component.h"
#include "../entity/components/RenderComponent.h"
#include "../entity/components/TerrainComponent.h"
#include "../entity/components/TransformComponent.h"

namespace sh
{
	namespace scene
	{
		Picker::Picker()
		{	
			m_material.reset(new sh::video::Material());
			m_material->SetRenderTechnique("ConstantColor3D.rt");
			m_params = m_material->GetCommonGpuParams();
			m_params->GetParam("matrixWVP", m_wvpMtrix);
			m_params->GetParam("color", m_color);

			m_mode = Mode::Color;
		}

		/////////////////////////////////////////////////////////

		Picker::~Picker()
		{
		}

		/////////////////////////////////////////////////////////

		void Picker::RegisterEntity(sh::Entity* entity)
		{
			if (!entity->GetComponent<RenderComponent>() && 
				!entity->GetComponent<TerrainComponent>())
				return;

			m_entities.push_back(entity);

		}

		/////////////////////////////////////////////////////////

		sh::Entity* Picker::TryToPick(sh::u32 x, sh::u32 y)
		{
			switch (m_mode)
			{
				case Mode::Color:
					return TryToPickByColor(x, y);
				case Mode::RayCast:
					return TryToPickByRayCast(x, y);
				default:
					return nullptr;
			}
		}

		/////////////////////////////////////////////////////////

		void Picker::Clear()
		{
			m_entities.clear();
		}

		/////////////////////////////////////////////////////////

		sh::Entity* Picker::TryToPickByColor(sh::u32 x, sh::u32 y)
		{
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			sh::scene::Camera* camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();
			sh::math::Matrix4f viewMatrix = camera->GetViewMatrix();
			sh::math::Matrix4f projectionMatrix = camera->GetProjectionMatrix();
			
			driver->BeginRendering();

			driver->SetRenderPipeline(m_material->GetRenderPipeline());

			for (size_t i = 0, sz = m_entities.size(); i < sz; ++i)
			{
				int r = (i & 0x000000FF) >>  0;
				int g = (i & 0x0000FF00) >>  8;
				int b = (i & 0x00FF0000) >> 16;
				sh::math::Vector4f color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

				m_color.Set(color);

				ModelPtr model;

				RenderComponent* renderComponent = m_entities[i]->GetComponent<RenderComponent>();
				if (renderComponent)
				{
					model = renderComponent->GetModel();
				}
				else
				{
					TerrainComponent* terrainComponent = m_entities[i]->GetComponent<TerrainComponent>();
					model = terrainComponent->GetModel();
				}
				

				size_t meshesCount = model->GetMeshesCount();
				for (size_t j = 0; j < meshesCount; ++j)
				{
					const auto& mesh = model->GetMesh(j);
					const auto& renderable = mesh->GetRanderable();
					sh::math::Matrix4f wvpMatrix = projectionMatrix * viewMatrix * mesh->GetWorldMatrix();
					wvpMatrix.Transpose();
					m_wvpMtrix.Set(wvpMatrix);

					const auto& vertexDeclaration = m_material->GetRenderPipeline()->GetVertexInputDeclaration();
					vertexDeclaration->Assemble(*(renderable->GetVertexBuffer()->GetVertexDeclaration()));

					driver->SetGpuParams(m_params);
					driver->SetVertexBuffer(renderable->GetVertexBuffer());
					driver->SetVertexDeclaration(vertexDeclaration);
					driver->SetIndexBuffer(renderable->GetIndexBuffer());				
					driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount());
				}			
			}
			
			unsigned char data[4];
			driver->GetPixelData(x, y, data);

			int pickedID = data[0] + data[1] * 256 + data[2] * 256*256;
			if (pickedID >= 0 && (size_t)pickedID < m_entities.size())
			{
				return m_entities[pickedID];
			}

			return nullptr;
		}

		/////////////////////////////////////////////////////////

		sh::Entity* Picker::TryToPickByRayCast(sh::u32 x, sh::u32 y)
		{
			sh::scene::Camera* camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();

			sh::math::Vector3f rayOrigin(0.0f);
			sh::math::Vector3f rayDirection(0.0f);

			float t0 = 0.0f;
			float t1 = 0.0f;

			camera->BuildRay(x, y, rayOrigin, rayDirection);

			for (size_t i = 0, sz = m_entities.size(); i < sz; ++i)
			{
				TransformComponent* transformComponent = m_entities[i]->GetComponent<sh::TransformComponent>();		
				if (!transformComponent)
					continue;

				RenderComponent* renderComponent = m_entities[i]->GetComponent<RenderComponent>();
				if (!renderComponent)
					continue;

				const auto& pos = transformComponent->GetPosition();
				const float radius = renderComponent->GetModel()->GetBoundingBox().GetRadius();

				int res = math::RayIntersectSphere(rayOrigin, rayDirection, pos, radius, t0, t1);
				if (res != 0)
					return m_entities[i];
			}

			return nullptr;
		}

		/////////////////////////////////////////////////////////
	}
}

