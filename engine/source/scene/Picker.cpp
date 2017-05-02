#include "Picker.h"
#include "../Device.h"
#include "../video/Driver.h"
#include "../video/Material.h"
#include "../video/RenderPipeline.h"
#include "../video/Renderable.h"
#include "../scene/Mesh.h"
#include "../scene/Model.h"
#include "../scene/Camera.h"
#include "../scene/SceneManager.h"
#include "../entity/Entity.h"
#include "../entity/Component.h"
#include "../entity/components/RenderComponent.h"
#include "../entity/components/TransformComponent.h"

namespace sh
{
	namespace scene
	{
		Picker::Picker()
		{	
			m_material.reset(new sh::video::Material());
			m_material->SetRenderTechnique("editor_base_color.xml");
			const auto& info = m_material->GetRenderPipeline()->GetAutoParamsInfo();	
			m_params = sh::video::GpuParams::Create(info);
			m_params->GetParam("matWVP", m_wvpMtrix);
			m_params->GetParam("color", m_color);
		}

		/////////////////////////////////////////////////////////

		Picker::~Picker()
		{
		}

		/////////////////////////////////////////////////////////

		void Picker::RegisterEntity(sh::Entity* entity)
		{
			if (!entity->GetComponent(sh::Component::Type::RENDER))
				return;

			m_entities.push_back(entity);

		}

		/////////////////////////////////////////////////////////

		sh::Entity* Picker::TryToPick(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height)
		{
			switch (m_mode)
			{
				case Mode::Color:
					return TryToPickByColor(x, y, width, height);
				case Mode::RayCast:
					return TryToPickByRayCast(x, y, width, height);
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

		sh::Entity* Picker::TryToPickByColor(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height)
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

				sh::RenderComponent* renderComponent = static_cast<sh::RenderComponent*>(m_entities[i]->GetComponent(sh::Component::Type::RENDER));		
				const auto& model = renderComponent->GetModel();

				size_t meshesCount = model->GetMeshesCount();
				for (size_t j = 0; j < meshesCount; ++j)
				{
					const auto& mesh = model->GetMesh(j);
					const auto& renderable = mesh->GetRanderable();
					sh::math::Matrix4f wvpMatrix = projectionMatrix * viewMatrix * mesh->GetWorldMatrix();
					m_wvpMtrix.Set(wvpMatrix);


					driver->SetGpuParams(m_params);
					driver->SetVertexBuffer(renderable->GetVertexBuffer());
					driver->SetVertexDeclaration(renderable->GetVertexInputDeclaration());
					driver->SetIndexBuffer(renderable->GetIndexBuffer());				
					driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount());
				}			
			}
			
			unsigned char data[4];
			driver->GetPixelData(x, y, width, height, data);

			int pickedID = data[0] + data[1] * 256 + data[2] * 256*256;
			if (pickedID >= 0 && (size_t)pickedID < m_entities.size())
			{
				return m_entities[pickedID];
			}

			return nullptr;
		}

		/////////////////////////////////////////////////////////

		sh::Entity* Picker::TryToPickByRayCast(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height)
		{
			sh::scene::Camera* camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();

			sh::math::Vector3f rayOrigin(0.0f);
			sh::math::Vector3f rayDirection(0.0f);

			float t0 = 0.0f;
			float t1 = 0.0f;

			camera->BuildRay(x, y, rayOrigin, rayDirection);

			for (size_t i = 0, sz = m_entities.size(); i < sz; ++i)
			{
				sh::TransformComponent* transformComponent = static_cast<sh::TransformComponent*>(m_entities[i]->GetComponent(sh::Component::Type::TRANSFORM));		
				const auto& pos = transformComponent->GetPosition();

				int res = math::RayIntersectSphere(rayOrigin, rayDirection, pos, 1.0f, t0, t1);
				if (res != 0)
					return m_entities[i];
			}

			return nullptr;
		}

		/////////////////////////////////////////////////////////
	}
}

