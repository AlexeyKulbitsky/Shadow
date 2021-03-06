#include "SceneManager.h"
#include "../resources/ResourceManager.h"
#include "../Device.h"
#include "../video/Driver.h"
#include "../video/RenderPipeline.h"
#include "../video/Sampler.h"
#include "ModelLoader/ModelLoader.h"
#include "../video/RenderTechnique.h"
#include "../video/Material.h"
#include "../video/CommandPool.h"
#include "Model.h"
#include "ModelBase.h"
#include "Mesh.h"
#include "MeshBase.h"
#include "Camera.h"
#include "Picker.h"
#include "../entity/System.h"
#include "../entity/systems/RenderSystem.h"
#include "../entity/systems/TransformSystem.h"
#include "../entity/systems/LightSystem.h"
#include "../entity/systems/ScriptSystem.h"
#include "../entity/Component.h"
#include "../entity/components/LightComponent.h"
#include "../entity/components/RenderComponent.h"
#include "../entity/components/TerrainComponent.h"
#include "../entity/components/TransformComponent.h"
#include "../entity/components/ScriptComponent.h"
#include "../entity/components/CameraComponent.h"

#include "../entity/Entity.h"
#include "../entity/ComponentsFactory.h"

#include "../gui/GuiManager.h"

#include "Scene.h"

#include <pugixml.hpp>

namespace sh
{
	namespace scene
	{
		math::Vector3 SceneManager::s_rightVector(1.0f, 0.0f, 0.0f);
		math::Vector3 SceneManager::s_upVector(0.0f, 1.0f, 0.0f);
		math::Vector3 SceneManager::s_frontVector(0.0f, 0.0f, -1.0f);

		//////////////////////////////////////////////////////////////////////////////////////////////

		SceneManager::SceneManager()
		{
// 			TransformSystem* transformSystem = new TransformSystem();
// 			transformSystem->Activate(true);
// 			m_systems.push_back(transformSystem);
// 
// 			RenderSystem* renderSystem = new RenderSystem();
// 			renderSystem->Activate(true);
// 			m_systems.push_back(renderSystem);
// 
// 			LightSystem* lightSystem = new LightSystem();
// 			lightSystem->Activate(true);
// 			m_systems.push_back(lightSystem);
// 
// 			ScriptSystem* scriptSystem = new ScriptSystem();
// 			scriptSystem->Activate(false);
// 			m_systems.push_back(scriptSystem);
// 			m_switchableSystems.push_back(scriptSystem);


			m_picker.reset(new Picker());

			TransformComponent::RegisterObject();
			LightComponent::RegisterObject();
			RenderComponent::RegisterObject();
			TerrainComponent::RegisterObject();
			CameraComponent::RegisterObject();
			ScriptComponent::RegisterObject();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		SceneManager::~SceneManager()
		{
			delete m_componentsFactory;
// 			for (size_t i = 0; i < m_systems.size(); ++i)
// 			{
// 				delete m_systems[i];
// 			}
// 
// 			for (size_t i = 0; i < m_entities.size(); ++i)
// 			{
// 				delete m_entities[i];
// 			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::LoadScene(const char* filename)
		{
			ClearScene();

			if (m_currentScene)
			{
				delete m_currentScene;
				m_currentScene = nullptr;
			}

			m_currentScene = new Scene();

			ResourceManager* resourceManager = Device::GetInstance()->GetResourceManager();
			auto file = io::FileSystem::GetInstance()->LoadFile(filename);

			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_buffer(file.GetData().data(), file.GetData().size());

			// Read scene
			pugi::xml_node scene = doc.child("scene");
			pugi::xml_node sceneNode = scene.first_child();
			while (sceneNode)
			{
				String sceneNodeName = sceneNode.name();

				// Read entities
				if (sceneNodeName == "entity")
				{
					sh::Entity* entity = new sh::Entity();
					entity->Load(sceneNode);

					// Add entity to entities list
					AddEntity(entity);

					m_currentScene->AddEntity(entity);
				}

				// Read next object
				sceneNode = sceneNode.next_sibling();
			}

			size_t entitiesCount = m_currentScene->GetEntitiesCount();
			for (size_t i = 0; i < entitiesCount; ++i)
			{
				m_picker->RegisterEntity(m_currentScene->GetEntity(i));
			}

// 			size_t entitiesCount = m_entities.size();
// 			for (size_t i = 0; i < entitiesCount; ++i)
// 			{
// 				m_picker->RegisterEntity(m_entities[i]);
// 			}

			SetSystemsState(sh::SystemState::Stopped);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::SaveScene(const char* filename)
		{
			if (!m_currentScene)
				return;

			pugi::xml_document doc;

			// Save scene
			pugi::xml_node sceneNode = doc.append_child("scene");
// 			for (size_t i = 0U, sz = m_entities.size(); i < sz; ++i)
// 			{
// 				m_entities[i]->Save(sceneNode);
// 			}

			size_t entitiesCount = m_currentScene->GetEntitiesCount();
			for (size_t i = 0; i < entitiesCount; ++i)
			{
				m_currentScene->GetEntity(i)->Save(sceneNode);
			}

			doc.save_file(filename);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		Scene* SceneManager::GetCurrentScene()
		{
			return m_currentScene;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////
		
		void SceneManager::SetCurrentScene(Scene* scene)
		{
			m_currentScene = scene;

			// Clear picker from previous entities
			m_picker->Clear();

			if (!m_currentScene)
				return;

			size_t entitiesCount = m_currentScene->GetEntitiesCount();
			for (size_t i = 0; i < entitiesCount; ++i)
			{
				m_picker->RegisterEntity(m_currentScene->GetEntity(i));
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::ClearScene()
		{
// 			for (auto system : m_systems)
// 			{
// 				system->Clear();
// 			}
// 
// 			for (size_t i = 0, sz = m_entities.size(); i < sz; ++i)
// 			{
// 				delete m_entities[i];
// 			}
// 			m_entities.clear();

			if (m_currentScene)
				m_currentScene->Clear();

			m_picker->Clear();
		}
        
        //////////////////////////////////////////////////////////////////////////////////////////////

        
        void SceneManager::AddEntity(Entity* entity)
        {
            m_entities.push_back(entity);
            
        }
        
        //////////////////////////////////////////////////////////////////////////////////////////////

        
        void SceneManager::RemoveEntity(Entity* entity)
        {
            for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                if ((*it) == entity)
                {
                    m_entities.erase(it);
                    return;
                }
            }
        }

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::RegisterEntity(Entity* entity)
		{
// 			for (auto system : m_systems)
// 			{
// 				system->RegisterEntity(entity);
// 			}
			if (m_currentScene)
			{
				m_currentScene->RegisterEntity(entity);
			}
		}
        
        void SceneManager::UnregisterEntity(Entity* entity)
        {
            if (m_currentScene)
            {
                m_currentScene->UnregisterEntity(entity);
            }
        }

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::RegisterComponent(Component* component)
		{
			for (auto system : m_systems)
			{
				system->RegisterComponent(component);
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::SetCamera(CameraPtr camera)
		{
			m_camera = camera;
		}
        
        //////////////////////////////////////////////////////////////////////////////////////////////
        
        const CameraPtr& SceneManager::GetCamera() const
        {
            return m_camera;
            
        }

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::Update(f32 deltaTime)
		{
			m_camera->Update();

			// Update all systems
// 			for (auto system : m_systems)
// 			{
// 				if (system->GetState() == SystemState::Running)
// 					system->Update(deltaTime);
// 			}

			if (m_currentScene)
				m_currentScene->Update(deltaTime);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::OnWindowResized(int width, int height)
		{
			m_camera->SetProjection(3.1415926535f / 3.0f,
									static_cast<f32>( width ),
									static_cast<f32>( height ), 0.1f, 1000.0f);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::OnSurfaceChanged(void*, int width, int height)
		{
			OnWindowResized(width, height);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::SetSystemsState(SystemState state)
		{
// 			for (auto system : m_switchableSystems)
// 			{
// 				system->SetState(state);
// 			}
			if (m_currentScene)
				m_currentScene->SetSystemsState(state);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////
	}
}
