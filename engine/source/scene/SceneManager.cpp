#include "SceneManager.h"
#include "../resources/ResourceManager.h"
#include "../Device.h"
#include "../video/Driver.h"
#include "../video/RenderPass.h"
#include "../video/ShaderProgram.h"
#include "../video/UniformBuffer.h"
#include "../video/Uniform.h"
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
			m_commandPool = new video::CommandPool();

			TransformSystem* transformSystem = new TransformSystem();
			transformSystem->AddComponentType(Component::Type::TRANSFORM);
			m_systems.push_back(transformSystem);

			RenderSystem* renderSystem = new RenderSystem();
			renderSystem->AddComponentType(Component::Type::RENDER);
			m_systems.push_back(renderSystem);

			LightSystem* lightSystem = new LightSystem();
			lightSystem->AddComponentType(Component::Type::LIGHT);
			m_systems.push_back(lightSystem);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		SceneManager::~SceneManager()
		{

		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::LoadScene(const char* filename)
		{
			ResourceManager* resourceManager = Device::GetInstance()->GetResourceManager();

			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filename);
			pugi::xml_node firstChild = doc.first_child();

			pugi::xml_node entityNode = firstChild.child("entity");
			while (entityNode)
			{
				sh::Entity* entity = new sh::Entity();

				// Read name
				pugi::xml_attribute nameAttribute = entityNode.attribute("name");
				if (nameAttribute)
				{
					printf("Entity %s", nameAttribute.as_string());
				}

				pugi::xml_node componentNode = entityNode.child("component");

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

				// Read next object
				entityNode = entityNode.next_sibling();
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::SetCamera(Camera* camera)
		{
			m_camera = camera;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		void SceneManager::Render()
		{
			for (size_t i = 0, sz = m_models.size(); i < sz; ++i)
			{
				m_models[i]->UpdateTransformationUniforms();
			}

			m_commandPool->Submit();
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