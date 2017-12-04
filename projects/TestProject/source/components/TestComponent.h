#ifndef TEST_COMPONENT_INCLUDE
#define TEST_COMPONENT_INCLUDE

#include <Shadow.h>

class TestComponent : public sh::Component
{
	SHADOW_OBJECT(TestComponent)
public:
	static size_t GetTypeId() { static size_t id = sh::ComponentTypeIdGenerator::GetAvailableId(); return id; } 
	virtual size_t GetId() override { return GetTypeId(); }

public:
	static void RegisterObject();

	const sh::math::Vector3& GetVector3Value() const;
	void SetVector3Vaue(const sh::math::Vector3& value);

private:
	sh::math::Vector3 m_vector3Value;
};

#endif