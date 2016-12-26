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

			void SetCamera(Camera* camera);
			Camera* GetCamera() { return m_camera; }

			void Render();
			void Update(f32 deltaTime = 0.0f);
			void OnEvent(const Event& e);

		private:
			static math::Vector3f	s_frontVector;
			static math::Vector3f	s_upVector;
			static math::Vector3f	s_rightVector;

			Camera* m_camera;
			std::vector<Model*> m_models;

			std::vector<System*> m_systems;

			video::CommandPool* m_commandPool;
		};
	}
}

#endif
