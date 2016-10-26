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
			static const math::vec3f& GetFrontVector() { return s_frontVector; }
			static const math::vec3f& GetUpVector() { return s_upVector; }
			static const math::vec3f& GetRightVector() { return s_rightVector; }
		private:
			static math::vec3f	s_frontVector;
			static math::vec3f	s_upVector;
			static math::vec3f	s_rightVector;

		};
	}
}

#endif
