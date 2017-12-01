#ifndef SHADOW_XML_SERIALIZER_INCLUDE
#define SHADOW_XML_SERIALIZER_INCLUDE

#include "Serializer.h"

namespace pugi
{
	class xml_node;
}

namespace sh
{

	class SHADOW_API XMLSerializer : public Serializer
	{
	public:
		void Serialize(Serializable* serializable, pugi::xml_node& node);
		void Deserialize(Serializable* serializable, const pugi::xml_node& node);
	};

} // sh

#endif
