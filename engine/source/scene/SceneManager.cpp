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
#include "../entity/Entity.h"
#include "../entity/ComponentsFactory.h"
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
			transformSystem->AddComponentType(Component::Type::TRANSFORM);
			m_systems.push_back(transformSystem);

			RenderSystem* renderSystem = new RenderSystem();
			renderSystem->AddComponentType(Component::Type::RENDER);
			m_systems.push_back(renderSystem);

			LightSystem* lightSystem = new LightSystem();
			lightSystem->AddComponentType(Component::Type::LIGHT);
			m_systems.push_back(lightSystem);

			m_picker.reset(new Picker());
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

			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filename);
			
			// Read materials
			pugi::xml_node materialsNode = doc.child("materials");
			pugi::xml_node materialNode = materialsNode.first_child();
			while (materialNode)
			{
				video::MaterialPtr material(new video::Material());
				material->Load(materialNode);
				resourceManager->AddMaterial(material);
				materialNode = materialNode.next_sibling();
			}

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
						Component::Type componentType;
						Component* component = nullptr;

						if (nameStr == "transform")
							componentType = Component::Type::TRANSFORM;
						else if (nameStr == "render")
							componentType = Component::Type::RENDER;
						else if (nameStr == "light")
							componentType = Component::Type::LIGHT;
						else
							componentNode = componentNode.next_sibling();

						//component = Component::Create(componentType);
						component = m_componentsFactory->CreateComponent(componentType);
						component->Load(componentNode);

						entity->AddComponent(component);

						for (auto system : m_systems)
						{
							system->RegisterEntity(entity);
						}					

						componentNode = componentNode.next_sibling();
					}

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

			// Save materials
			ResourceManager* resourceManager = Device::GetInstance()->GetResourceManager();			
			size_t materialsCount = resourceManager->GetMaterialsCount();
			pugi::xml_node materialsNode = doc.append_child("materials");
			for (size_t i = 0U; i < materialsCount; ++i)
			{
				resourceManager->GetMaterial(i)->Save(materialsNode);
			}

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

		void SceneManager::OnEvent(const Event& e)
		{
			switch (e.type)
			{
			case EventType::KEYBOARD_INPUT_EVENT:
				break;
			case EventType::MOUSE_INPUT_EVENT:
				break;
			default:
				break;
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////////////
	}
}