#ifndef SHADOW_SERIALIZABLE_INCLUDE
#define SHADOW_SERIALIZABLE_INCLUDE

#include "../Globals.h"

namespace sh
{

	class SHADOW_API Serializable
	{
		SHADOW_OBJECT(Serializable)
	public:
		virtual void PrintDebugInfo() {}
		virtual void Serialize();
	};

} // sh



#endif