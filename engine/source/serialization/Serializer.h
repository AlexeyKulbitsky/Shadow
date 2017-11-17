#ifndef SHADOW_SERIALIZER_INCLUDE
#define SHADOW_SERIALIZER_INCLUDE

namespace sh
{

	class Serializable;

	class Serializer
	{
	public:
		void Serialize(Serializable* serializable);
	};

} // sh

#endif
