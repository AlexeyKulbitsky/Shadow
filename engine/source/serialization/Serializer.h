#ifndef SHADOW_SERIALIZER_INCLUDE
#define SHADOW_SERIALIZER_INCLUDE

#include "../Globals.h"

namespace sh
{

	class Serializable;

	class SHADOW_API Serializer
	{
	public:
        virtual ~Serializer() = default;
		virtual void Serialize(Serializable* serializable);
        virtual void Deserialize(Serializable* serializable);
        
        virtual Serializer* Child() = 0;
	};

} // sh

#endif
