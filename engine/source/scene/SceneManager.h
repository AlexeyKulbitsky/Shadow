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

		class SHADOW_API SceneManager
		{
		public:
			static const math::Vector3& GetFrontVector() { return s_frontVector; }
			static const math::Vector3& GetUpVector() { return s_upVector; }
			static const math::Vector3& GetRightVector() { return s_rightVector; }

			SceneManager();
			~SceneManager();

			void LoadScene(const char* filename);
			void SaveScene(const char* filename);

			void ClearScene();

			void AddEntity(Entity* entity) { m_entities.push_back(entity); }
			void RegisterEntity(Entity* entity);
			void RegisterComponent(Component* component);
			size_t GetEntitiesCount() const { return m_entities.size(); }
			Entity* GetEntity(size_t index) { return m_entities[index]; }

			void SetComponentsFactory(ComponentsFactory* factory) { m_componentsFactory = factory; }

			void SetCamera(Camera* camera);
			Camera* GetCamera() { return m_camera; }

			const PickerPtr& GetPicker() const { return m_picker; }
			void Update(f32 deltaTime = 0.0f);

			void OnWindowResized(int width, int height);
			void OnSurfaceChanged(void* surface, int width, int height);

			void ActivateSystems(bool activate);

		private:
			static math::Vector3	s_frontVector;
			static math::Vector3	s_upVector;
			static math::Vector3	s_rightVector;

			Camera* m_camera;

			ComponentsFactory* m_componentsFactory = nullptr;
			std::vector<System*> m_systems;
			std::vector<Entity*> m_entities;

			std::vector<System*> m_switchableSystems;

			PickerPtr m_picker;
		};
	}
}

#endif
