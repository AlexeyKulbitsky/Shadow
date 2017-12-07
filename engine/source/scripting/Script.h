#ifndef SHADOW_SCRIPT_INCLUDE
#define SHADOW_SCRIPT_INCLUDE

#include "../serialization/Serializable.h"

namespace sh
{
	class ScriptComponent;

	class SHADOW_API Script : public Serializable
	{
		SHADOW_OBJECT(Script)
	public:
		virtual ~Script() {}
		virtual void Update(float deltaTime) {}

		template<typename T>
		T* GetComponent()
		{
			return m_scriptComponent->GetParentEntity()->GetComponent<T>();
		}
	private:
		friend class ScriptComponent;
		ScriptComponent* m_scriptComponent = nullptr;
	};

} // sh

#endif