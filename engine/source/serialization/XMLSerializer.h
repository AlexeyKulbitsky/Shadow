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
        XMLSerializer() = default;
        ~XMLSerializer();
        XMLSerializer(pugi::xml_node* root);
        virtual void Serialize(Serializable* serializable) override;
        virtual void Deserialize(Serializable* serializable) override;
        virtual Serializer* Child() override;
        
		void Serialize(Serializable* serializable, pugi::xml_node& node);
		void Deserialize(Serializable* serializable, const pugi::xml_node& node);
        
    private:
        pugi::xml_node* m_root = nullptr;
        //pugi::xml_node* m_currentNode = nullptr;
	};

} // sh

#endif
