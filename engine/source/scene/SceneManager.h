#ifndef SHADOW_SCENE_MANAGER_INCLUDE
#define SHADOW_SCENE_MANAGER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class CommandPool;
	}

	class System;
	class Entity;
	class ComponentsFactory;

	namespace scene
	{
		class Model;
		class Camera;

		class SceneManager
		{
		public:
			static const math::Vector3f& GetFrontVector() { return s_frontVector; }
			static const math::Vector3f& GetUpVector() { return s_upVector; }
			static const math::Vector3f& GetRightVector() { return s_rightVector; }

			SceneManager();
			~SceneManager();

			void LoadScene(const char* filename);
			void SaveScene(const char* filename);

			void ClearScene();

			void AddModel(Model* model) { m_models.push_back(model); }
			size_t GetModelsCount() const { return m_models.size(); }
			Model* GetModel(size_t index) { return m_models[index]; }

			void AddEntity(Entity* entity) { m_entities.push_back(entity); }
			size_t GetEntitiesCount() const { return m_entities.size(); }
			Entity* GetEntity(size_t index) { return m_entities[index]; }

			void SetComponentsFactory(ComponentsFactory* factory) { m_componentsFactory = factory; }

			void SetCamera(Camera* camera);
			Camera* GetCamera() { return m_camera; }

			const PickerPtr& GetPicker() const { return m_picker; }
			void Update(f32 deltaTime = 0.0f);
			void OnEvent(const Event& e);

		private:
			static math::Vector3f	s_frontVector;
			static math::Vector3f	s_upVector;
			static math::Vector3f	s_rightVector;

			Camera* m_camera;
			std::vector<Model*> m_models;

			ComponentsFactory* m_componentsFactory = nullptr;
			std::vector<System*> m_systems;
			std::vector<Entity*> m_entities;

			PickerPtr m_picker;
		};
	}
}

#endif
