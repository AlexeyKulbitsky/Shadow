#include "Scene.h"

#include "../entity/Entity.h"
#include "../entity/System.h"
#include "../entity/systems/TransformSystem.h"
#include "../entity/systems/RenderSystem.h"
#include "../entity/systems/LightSystem.h"
#include "../entity/systems/ScriptSystem.h"

namespace sh
{

namespace scene
{

	Scene::Scene() 
	{
		TransformSystem* transformSystem = new TransformSystem();
		transformSystem->Activate(true);
		m_systems.push_back(transformSystem);

		RenderSystem* renderSystem = new RenderSystem();
		renderSystem->Activate(true);
		m_systems.push_back(renderSystem);

		LightSystem* lightSystem = new LightSystem();
		lightSystem->Activate(true);
		m_systems.push_back(lightSystem);

		ScriptSystem* scriptSystem = new ScriptSystem();
		scriptSystem->Activate(false);
		//m_systems.push_back(scriptSystem);
		m_switchableSystems.push_back(scriptSystem);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	Scene::~Scene() 
	{
		for (size_t i = 0; i < m_systems.size(); ++i)
		{
			delete m_systems[i];
		}

		for (size_t i = 0; i < m_entities.size(); ++i)
		{
			delete m_entities[i];
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	void Scene::AddEntity(Entity* entity) 
	{ 
		m_entities.push_back(entity); 
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	size_t Scene::GetEntitiesCount() const
	{
		return m_entities.size();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	Entity* Scene::GetEntity(size_t index)
	{
		if (index >= m_entities.size())
			return nullptr;
		return m_entities[index];
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	void Scene::RegisterEntity(Entity* entity)
	{
		for (auto system : m_systems)
		{
			system->RegisterEntity(entity);
		}

		for (auto system : m_switchableSystems)
		{
			system->RegisterEntity(entity);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	void Scene::SetSystemsState(SystemState state)
	{
		for (auto system : m_switchableSystems)
		{
			system->SetState(state);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	void Scene::Update(float deltaTime/* = 0.0f*/)
	{
		// Update all systems
		for (auto system : m_systems)
		{
			system->Update(deltaTime);
		}

		for (auto system : m_switchableSystems)
		{
			if (system->GetState() == SystemState::Running)
				system->Update(deltaTime);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	
	void Scene::Clear()
	{
		for (auto system : m_systems)
		{
			system->Clear();
		}

		for (auto system : m_switchableSystems)
		{
			system->Clear();
		}

		for (size_t i = 0, sz = m_entities.size(); i < sz; ++i)
		{
			delete m_entities[i];
		}
		m_entities.clear();
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

	Scene* Scene::Clone()
	{
		Scene* scene = new Scene();

		for (size_t i = 0, sz = m_entities.size(); i < sz; ++i)
		{
			Entity* entity = m_entities[i]->Clone();

			scene->m_entities.push_back(entity);
			scene->RegisterEntity(entity);
		}

		return scene;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////

} // scene

} // sh