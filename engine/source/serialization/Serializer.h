#ifndef SHADOW_SERIALIZER_INCLUDE
#define SHADOW_SERIALIZER_INCLUDE

#include "../Globals.h"

namespace sh
{

	class Serializable;

	class SHADOW_API Serializer
	{
	public:
		virtual void Serialize(Serializable* serializable);
	};

} // sh

#endif
