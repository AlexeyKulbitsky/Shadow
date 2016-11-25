#ifndef SHADOW_SCENE_MANAGER_INCLUDE
#define SHADOW_SCENE_MANAGER_INCLUDE

#include "../math/smath.h"

namespace sh
{
	namespace scene
	{
		class SceneManager
		{
		public:
			static const math::Vector3f& GetFrontVector() { return s_frontVector; }
			static const math::Vector3f& GetUpVector() { return s_upVector; }
			static const math::Vector3f& GetRightVector() { return s_rightVector; }

			void Render();
		private:
			static math::Vector3f	s_frontVector;
			static math::Vector3f	s_upVector;
			static math::Vector3f	s_rightVector;

		};
	}
}

#endif
