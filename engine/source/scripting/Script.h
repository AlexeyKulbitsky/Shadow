#ifndef SHADOW_SCRIPT_INCLUDE
#define SHADOW_SCRIPT_INCLUDE

#include "../serialization/Serializable.h"

namespace sh
{

	class SHADOW_API Script : public Serializable
	{
		SHADOW_OBJECT(Script)
	public:
		virtual ~Script() {}
		virtual void Update(float deltaTime) {}
	};

} // sh

#endif