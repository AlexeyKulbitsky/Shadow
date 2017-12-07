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
// 	std::cout << "Updating SCRIPT A" << std::endl;
// 	std::cout << "Vector3 value: " << m_vector3.x << " " << m_vector3.y << " " << m_vector3.z << std::endl;
// 	std::cout << "Quaternion value: " << m_quaternion.x << " " << m_quaternion.y << " " << m_quaternion.z << " " << m_quaternion.w << std::endl;
// 	std::cout << "------------------------------------" << std::endl;

	auto transformComponent = GetComponent<sh::TransformComponent>();
	if (transformComponent)
		transformComponent->SetPosition(transformComponent->GetPosition() + sh::math::Vector3(0.005f, 0.0f, 0.0f));
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