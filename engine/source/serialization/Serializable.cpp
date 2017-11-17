#include "Serializable.h"
#include "Serializer.h"

namespace sh
{

	void Serializable::Serialize()
	{
		Serializer serializer;
		serializer.Serialize(this);
	}

} // sh
