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

	void RenderSystem::AddEntity(Entity* entity)
	{
		m_entities.push_back(entity);

		RenderComponent* renderComponent = static_cast<RenderComponent*>( entity->GetComponent(Component::Type::Render) );
		if (renderComponent)
		{
			const scene::ModelPtr& model = renderComponent->GetModel();

			for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
			{
				m_batchManager->AddMesh(model->GetMesh(i));
			}
		}
		
		TerrainComponent* terrainComponent = static_cast<TerrainComponent*>(entity->GetComponent(Component::Type::Terrain));
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
		scene::Camera* camera = Device::GetInstance()->GetSceneManager()->GetCamera();
		sh::math::Matrix4f viewMatrix = camera->GetViewMatrix();
		sh::math::Matrix4f projectionMatrix = camera->GetProjectionMatrix();

		for (auto entity : m_entities)
		{
			scene::Model* model = nullptr;

			// Process if it is usual model
			RenderComponent* renderComponent = static_cast<RenderComponent*>( entity->GetComponent(Component::Type::Render) );
			if (renderComponent)
			{
				TransformComponent* transformComponent = static_cast<TransformComponent*>(entity->GetComponent(Component::Type::Transform));
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
				TerrainComponent* terrainComponent = static_cast<TerrainComponent*>(entity->GetComponent(Component::Type::Terrain));
				if (terrainComponent)
				{
					terrainComponent->GetModel()->SetPosition(math::Vector3f(0.0f));
					terrainComponent->GetModel()->SetWorldMatrix(math::Matrix4f::Identity());
					model = terrainComponent->GetModel().get();
				}
			}

			size_t meshesCount = model->GetMeshesCount();
			for (size_t i = 0; i < meshesCount; ++i)
			{
				const auto& renderable = model->GetMesh(i)->GetRanderable();
				const auto& params = renderable->GetAutoParams();
				for (size_t paramIdx = 0; paramIdx < params->GetParamsCount(); ++paramIdx)
				{
					auto& param = params->GetParam(paramIdx);
					switch (param.GetType())
					{
						case MaterialParamType::MatrixWorld:
							param.Set(renderable->GetMatrix());
							break;
						case MaterialParamType::MatrixView:
							param.Set(viewMatrix);
							break;
						case MaterialParamType::MatrixViewRotation:
						{
							param.Set(camera->GetRotationMatrix());
						}
							break;
						case MaterialParamType::MatrixViewRotationProjection:
							param.Set((projectionMatrix * camera->GetRotationMatrix()).GetTransposed());
							break;
						case MaterialParamType::MatrixProjection:
							param.Set(projectionMatrix);
							break;
						case MaterialParamType::MatrixViewProjection:
						{
							math::Matrix4f viewProjection = projectionMatrix * viewMatrix;
							param.Set(viewProjection);
						}
							break;
						case MaterialParamType::MatrixWorldViewProjection:
						{
							math::Matrix4f wvp = projectionMatrix * viewMatrix * renderable->GetMatrix();
							//wvp.m[1][1] *= -1.0f;
							wvp.Transpose();
							param.Set(wvp);
						}
							break;
						default:
							break;
					}
				}
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