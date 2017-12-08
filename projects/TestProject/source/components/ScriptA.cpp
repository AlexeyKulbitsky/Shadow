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
		delta.z += 0.05f;
	}
	else if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_A))
	{
		delta.x -= 0.05f;
	}
	else if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_S))
	{
		delta.z -= 0.05f;
	}
	else if (inputManager->IsKeyPressed(sh::KeyCode::KEY_KEY_D))
	{
		delta.x += 0.05f;
	}

	auto transformComponent = GetComponent<sh::TransformComponent>();
	if (transformComponent)
		transformComponent->SetPosition(transformComponent->GetPosition() + delta);
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