#include "RenderSystem.h"
#include "../Entity.h"
#include "../components/RenderComponent.h"
#include "../components/TransformComponent.h"
#include "../components/TerrainComponent.h"
#include "../../scene/Model.h"
#include "../../scene/Mesh.h"
#include "../../scene/SceneManager.h"
#include "../../scene/Camera.h"
#include "../../video/RenderBatchManager.h"
#include "../../video/Renderable.h"
#include "../../video/MaterialParams.h"

#include "../../Device.h"
#include"../../video/Driver.h"

namespace sh
{
	RenderSystem::RenderSystem()
	{
		m_batchManager = video::RenderBatchManager::GetInstance();
	}

	//////////////////////////////////////////////////////////////////

	RenderSystem::~RenderSystem()
	{
	}

	//////////////////////////////////////////////////////////////////

	void RenderSystem::RegisterEntity(Entity* entity)
	{
		if (entity->GetComponent<RenderComponent>() ||
			entity->GetComponent<TerrainComponent>())
		{
			AddEntity(entity);
		}
	}
	
	//////////////////////////////////////////////////////////////////

	void RenderSystem::RegisterComponent(Component* component)
	{
	}

	//////////////////////////////////////////////////////////////////

	void RenderSystem::AddEntity(Entity* entity)
	{
		auto it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it != m_entities.end())
			return;
		m_entities.push_back(entity);

		RenderComponent* renderComponent = entity->GetComponent<RenderComponent>();
		if (renderComponent)
		{
			const scene::ModelPtr& model = renderComponent->GetModel();

			for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
			{
				m_batchManager->AddMesh(model->GetMesh(i));
			}
		}
		
		TerrainComponent* terrainComponent = entity->GetComponent<TerrainComponent>();
		if (terrainComponent)
		{
			const scene::ModelPtr& model = terrainComponent->GetModel();

			for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
			{
				m_batchManager->AddMesh(model->GetMesh(i));
			}
		}
	}

	//////////////////////////////////////////////////////////////////

	void RenderSystem::RemoveEntity(Entity* entity)
	{
		for (size_t i = 0, sz = m_entities.size(); i < sz; ++i)
		{
			if (m_entities[i] == entity)
			{
				m_entities.erase(m_entities.begin() + i);
				return;
			}			
		}
	}

	//////////////////////////////////////////////////////////////////

	void RenderSystem::Update(f32 deltaTime)
	{
		m_batchManager->Clear();

		scene::Camera* camera = Device::GetInstance()->GetSceneManager()->GetCamera();
		sh::math::Matrix4 viewMatrix = camera->GetViewMatrix();
		sh::math::Matrix4 projectionMatrix = camera->GetProjectionMatrix();

		for (auto entity : m_entities)
		{
			scene::Model* model = nullptr;

			// Process if it is usual model
			RenderComponent* renderComponent = entity->GetComponent<RenderComponent>();
			if (renderComponent)
			{
				TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
				if (transformComponent)
				{
					renderComponent->GetModel()->SetPosition(transformComponent->GetPosition());
					renderComponent->GetModel()->SetWorldMatrix(transformComponent->GetWorldMatrix());
				}
				model = renderComponent->GetModel().get();
			}
			// Process if it is terrain
			else
			{
				TerrainComponent* terrainComponent = entity->GetComponent<TerrainComponent>();
				if (terrainComponent)
				{
					TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
					if (transformComponent)
					{
						terrainComponent->GetModel()->SetPosition(transformComponent->GetPosition());
						terrainComponent->GetModel()->SetWorldMatrix(transformComponent->GetWorldMatrix());
					}
					else
					{
						terrainComponent->GetModel()->SetPosition(math::Vector3(0.0f));
						terrainComponent->GetModel()->SetWorldMatrix(math::Matrix4::Identity());
					}
					model = terrainComponent->GetModel().get();
				}
			}

			if (!model)
				continue;

			size_t meshesCount = model->GetMeshesCount();
			for (size_t i = 0; i < meshesCount; ++i)
			{
				const auto& mesh = model->GetMesh(i);
				const auto& renderable = mesh->GetRanderable();
				const auto& params = renderable->GetAutoParams();
				for (size_t paramIdx = 0; paramIdx < params->GetParamsCount(); ++paramIdx)
				{
					auto param = params->GetParam(paramIdx);
					switch (param->GetUsage())
					{
						case MaterialParamUsage::MatrixWorld:
							param->Set(renderable->GetMatrix().GetTransposed());
							break;
						case MaterialParamUsage::MatrixView:
							param->Set(viewMatrix);
							break;
						case MaterialParamUsage::MatrixViewRotation:
						{
							param->Set(camera->GetRotationMatrix());
						}
							break;
						case MaterialParamUsage::MatrixViewRotationProjection:
							param->Set((projectionMatrix * camera->GetRotationMatrix()).GetTransposed());
							break;
						case MaterialParamUsage::MatrixProjection:
							param->Set(projectionMatrix);
							break;
						case MaterialParamUsage::MatrixViewProjection:
						{
							math::Matrix4 viewProjection = projectionMatrix * viewMatrix;
							param->Set(viewProjection);
						}
							break;
						case MaterialParamUsage::MatrixWorldViewProjection:
						{
							math::Matrix4 wvp = projectionMatrix * viewMatrix * renderable->GetMatrix();
							//wvp.m[1][1] *= -1.0f;
							wvp.Transpose();
							param->Set(wvp);
						}
							break;
						default:
							break;
					}
				}
				m_batchManager->AddMesh(mesh);
			}
		}

		m_batchManager->Submit();
	}

	//////////////////////////////////////////////////////////////////

	void RenderSystem::Clear()
	{
		m_batchManager->Clear();
		m_entities.clear();
	}
}