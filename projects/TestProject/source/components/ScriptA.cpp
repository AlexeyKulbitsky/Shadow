#include "ScriptA.h"

#include <iostream>

void ScriptA::RegisterObject()
{
	sh::ObjectFactory::GetInstance()->RegisterFactory<ScriptA>("Scripts");
	S_ACCESSOR_PROPERTY("Vector3", GetVector3, SetVector3);
	S_ACCESSOR_PROPERTY("Quaternion", GetQuaternion, SetQuaternion);
}

void ScriptA::Update(float dt)
{
	sh::InputManager* inputManager = sh::Device::GetInstance()->GetInputManager();
	sh::math::Vector3 delta(0.0f);

	if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_W))
	{
		delta.z += 0.5f;
	}
	if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_A))
	{
		delta.x -= 0.5f;
	}
	if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_S))
	{
		delta.z -= 0.5f;
	}
	if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_D))
	{
		delta.x += 0.5f;
	}

	auto cameraComponent = GetComponent<sh::CameraComponent>();
	if (cameraComponent)
	{
		auto camera = cameraComponent->GetCamera();
		if (!camera)
			return;

		sh::Device* device = sh::Device::GetInstance();
		sh::InputManager* inputManager = device->GetInputManager();
		sh::math::Vector2Int old = inputManager->GetMousePositionOld();
		sh::math::Vector2Int current = inputManager->GetMousePositionCurrent();
		sh::math::Vector2Int mouseDelta = current - old;

		std::cout << "Delta mouse: X: " << mouseDelta.x << " Y: " << mouseDelta.y << std::endl;
		float yAxisAngle = -(float)mouseDelta.x * 0.01f;
		sh::math::Quaternion yAxisRot;
		yAxisRot.SetFromAxisAngle(sh::scene::SceneManager::GetUpVector(), yAxisAngle);

		float xAxisangle = -(float)mouseDelta.y * 0.01f;
		sh::math::Quaternion xAxisRot;
		xAxisRot.SetFromAxisAngle(camera->GetRightVector(), xAxisangle);

		sh::math::Quaternion deltaRot = yAxisRot * xAxisRot;
		camera->SetRotation(deltaRot * camera->GetRotation());

		sh::math::Vector3 deltaPos = 
			camera->GetFrontVector() * delta.z +
			camera->GetRightVector() * delta.x;

		camera->SetPosition(camera->GetPosition() + deltaPos);
		auto sceneManager = sh::Device::GetInstance()->GetSceneManager();
		sceneManager->SetCamera(camera);
	}
}

sh::Script* ScriptA::Clone()
{
	ScriptA* script = new ScriptA();

	script->m_vector3 = m_vector3;
	script->m_quaternion = m_quaternion;

	return script;
}

void ScriptA::SetVector3(const sh::math::Vector3& value)
{
	m_vector3 = value;
}

const sh::math::Vector3& ScriptA::GetVector3() const
{
	return m_vector3;
}

void ScriptA::SetQuaternion(const sh::math::Quaternion& value)
{
	m_quaternion = value;
}

const sh::math::Quaternion& ScriptA::GetQuaternion() const
{
	return m_quaternion;
}