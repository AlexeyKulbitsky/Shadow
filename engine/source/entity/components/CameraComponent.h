#ifndef SHADOW_CAMERA_COMPONENT_INCLUDE
#define SHADOW_CAMERA_COMPONENT_INCLUDE

#include "../Component.h"

namespace sh
{

	namespace scene
	{
		class Camera;
	}

	class SHADOW_API CameraComponent : public Component
	{
		COMPONENT(CameraComponent)
	public:
		CameraComponent();
		virtual ~CameraComponent();

		static void RegisterObject();

		virtual Component* Clone() override;

		scene::Camera* GetCamera();

	private:
		scene::Camera* m_camera = nullptr;
		friend class CameraSystem;
	};

} // sh

#endif
