#ifndef SHADOW_SERIALIZABLE_INCLUDE
#define SHADOW_SERIALIZABLE_INCLUDE

#include "../Globals.h"

namespace sh
{

	class SHADOW_API Serializable
	{
    public:
        using ClassType = Serializable;
        using BaseClassType = Serializable;
        static const char* GetTypeNameStatic() { return "Serializable"; }
        virtual const char* GetTypeName() { return GetTypeNameStatic(); }
        
		virtual void PrintDebugInfo() {}
		virtual void Serialize();
	};

} // sh



#endif
