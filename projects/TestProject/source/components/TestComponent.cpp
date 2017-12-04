#include "TestComponent.h"

void TestComponent::RegisterObject()
{
	sh::ObjectFactory::GetInstance()->RegisterFactory<TestComponent>();
	S_ACCESSOR_PROPERTY("Vector3", GetVector3Value, SetVector3Vaue);
}

const sh::math::Vector3& TestComponent::GetVector3Value() const
{
	return m_vector3Value;
}

void TestComponent::SetVector3Vaue(const sh::math::Vector3& value)
{
	m_vector3Value = value;
}