#ifndef SHADOW_XML_SERIALIZER_INCLUDE
#define SHADOW_XML_SERIALIZER_INCLUDE

#include "Serializer.h"

namespace sh
{

	class XMLSerializer : public Serializer
	{
	public:
		void Serialize(Serializable* serializable, pugi::xml_node& node);
	};

} // sh

#endif
