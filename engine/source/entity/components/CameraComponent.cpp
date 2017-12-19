#include "CameraComponent.h"

#include "../../scene/Camera.h"
#include "../../serialization/ObjectFactory.h"

#include "../../scene/SceneManager.h"
#include "../../Device.h"

namespace sh
{

	CameraComponent::CameraComponent()
	{
		m_camera = Device::GetInstance()->GetSceneManager()->GetCamera()->Clone();
	}

	CameraComponent::~CameraComponent()
	{
		if (m_camera)
		{
			delete m_camera;
			m_camera = nullptr;
		}
	}

	void CameraComponent::RegisterObject()
	{
		ObjectFactory::GetInstance()->RegisterFactory<CameraComponent>("Components");
	}

	Component* CameraComponent::Clone() 
	{
		CameraComponent* component = new CameraComponent();
		
		component->m_camera = m_camera->Clone();
		
		return component;
	}

	sh::scene::Camera* CameraComponent::GetCamera()
	{
		return m_camera;
	}

} // sh