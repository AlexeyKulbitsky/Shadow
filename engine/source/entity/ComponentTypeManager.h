#ifndef SHADOW_COMPONENT_TYPE_MANAGER_INCLUDE
#define SHADOW_COMPONENT_TYPE_MANAGER_INCLUDE

#include "Component.h"

namespace sh
{

	class SHADOW_API ComponentTypeManager
	{
	public:
		
		template<typename T>
		static void* GetTypeInfo()
		{
			return nullptr;
		}

	private:

	};

} // sh

#endif
