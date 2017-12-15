#ifndef SCRIPT_A_INCLUDE
#define SCRIPT_A_INCLUDE

#include <Shadow.h>

class ScriptA : public sh::Script
{
	SHADOW_OBJECT(ScriptA)
public:
	static void RegisterObject();
	virtual void Update(float dt) override;

	virtual sh::Script* Clone() override;

	void SetVector3(const sh::math::Vector3& value);
	const sh::math::Vector3& GetVector3() const;

	void SetQuaternion(const sh::math::Quaternion& value);
	const sh::math::Quaternion& GetQuaternion() const;

private:
	sh::math::Vector3 m_vector3;
	sh::math::Quaternion m_quaternion;
};

#endif
