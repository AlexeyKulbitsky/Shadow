#include "RenderSystem.h"
#include "../Entity.h"
#include "../components/RenderComponent.h"
#include "../components/TransformComponent.h"
#include "../../scene/Model.h"
#include "../../video/RenderBatchManager.h"

#include "../../Device.h"
#include"../../video/Driver.h"

namespace sh
{
	RenderSystem::RenderSystem()
	{
		m_batchManager = Device::GetInstance()->GetDriver()->CreateRenderBatchManager();
	}

	//////////////////////////////////////////////////////////////////

	RenderSystem::~RenderSystem()
	{
		delete m_batchManager;
	}

	//////////////////////////////////////////////////////////////////

	void RenderSystem::AddEntity(Entity* entity)
	{
		m_entities.push_back(entity);

		RenderComponent* renderComponent = static_cast<RenderComponent*>( entity->GetComponent(Component::Type::RENDER) );
		const scene::ModelPtr& model = renderComponent->GetModel();

		for (size_t i = 0, sz = model->GetMeshesCount(); i < sz; ++i)
		{
			m_batchManager->AddMesh(model->GetMesh(i));
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
		for (auto entity : m_entities)
		{
			RenderComponent* renderComponent = static_cast<RenderComponent*>( entity->GetComponent(Component::Type::RENDER) );
			TransformComponent* transformComponent = static_cast<TransformComponent*>( entity->GetComponent(Component::Type::TRANSFORM) );
			if (transformComponent)
			{
				renderComponent->GetModel()->SetPosition(transformComponent->GetPosition());
				renderComponent->GetModel()->SetWorldMatrix(transformComponent->GetWorldMatrix());
			}			
			renderComponent->GetModel()->UpdateTransformationUniforms();
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