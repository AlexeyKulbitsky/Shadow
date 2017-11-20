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
#include "../entity/Component.h"
#include "../entity/components/LightComponent.h"
#include "../entity/components/RenderComponent.h"
#include "../entity/components/TerrainComponent.h"
#include "../entity/components/TransformComponent.h"

#include "../entity/Entity.h"
#include "../entity/ComponentsFactory.h"

#include "../gui/GuiManager.h"

#include <pugixml.hpp>

namespace sh
{
	namespace scene
	{
		math::Vector3f SceneManager::s_rightVector(1.0f, 0.0f, 0.0f);
		math::Vector3f SceneManager::s_upVector(0.0f, 1.0f, 0.0f);
		math::Vector3f SceneManager::s_frontVector(0.0f, 0.0f, -1.0f);

		//////////////////////////////////////////////////////////////////////////////////////////////

		SceneManager::SceneManager()
		{
			TransformSystem* transformSystem = new TransformSystem();
			m_systems.push_back(transformSystem);

			RenderSystem* renderSystem = new RenderSystem();
			m_systems.push_back(renderSystem);

			LightSystem* lightSystem = new LightSystem();
			m_systems.push_back(lightSystem);

			m_picker.reset(new Picker());

			TransformComponent::RegisterObject();
			LightComponent::RegisterObject();
			RenderComponent::RegisterObject();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		SceneManager::~SceneManager()
		{
			delete m_camera;
			delete m_componentsFactory;
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

		void SceneManager::LoadScene(const char* filename)
		{
			ClearScene();

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

					// Read name
					pugi::xml_attribute nameAttribute = sceneNode.attribute("name");
					if (nameAttribute)
					{
						String name = nameAttribute.as_string();
						printf("Entity %s", name.c_str());
						entity->SetName(name);
					}

					pugi::xml_node componentNode = sceneNode.child("component");

					while (componentNode)
					{
						pugi::xml_attribute componentName = componentNode.attribute("name");
						String nameStr = componentName.as_string();
						Component* component = nullptr;

						if (nameStr == "transform")
							component = new TransformComponent();
						else if (nameStr == "render")
							component = new RenderComponent();
						else if (nameStr == "light")
							component = new LightComponent();
						else if (nameStr == "terrain")
							component = new TerrainComponent();
						else
							componentNode = componentNode.next_sibling();

						component->Load(componentNode);

						entity->AddComponent(component);

						componentNode = componentNode.next_sibling();
					}

					// Register entity in all systems
					RegisterEntity(entity);
					// Add entity to entities list
					AddEntity(entity);
				}

				// Read next object
				sceneNode = sceneNode.next_sibling();
			}


			size_t entitiesCount = m_entities.size();
			for (size_t i = 0; i < entitiesCount; ++i)
			{
				m_picker->RegisterEntity(m_entities[i]);
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::SaveScene(const char* filename)
		{
			pugi::xml_document doc;

			// Save scene
			pugi::xml_node sceneNode = doc.append_child("scene");
			for (size_t i = 0U, sz = m_entities.size(); i < sz; ++i)
			{
				m_entities[i]->Save(sceneNode);
			}

			doc.save_file(filename);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::ClearScene()
		{
			for (auto system : m_systems)
			{
				system->Clear();
			}

			for (size_t i = 0, sz = m_entities.size(); i < sz; ++i)
			{
				delete m_entities[i];
			}
			m_entities.clear();

			m_picker->Clear();
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::RegisterEntity(Entity* entity)
		{
			for (auto system : m_systems)
			{
				system->RegisterEntity(entity);
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::SetCamera(Camera* camera)
		{
			m_camera = camera;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::Update(f32 deltaTime)
		{
			m_camera->Update();

			// Update all systems
			for (auto system : m_systems)
			{
				system->Update(deltaTime);
			}
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

	}
}