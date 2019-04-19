#ifndef SHADOW_RESOURCE_MANAGER_INCLUDE
#define SHADOW_RESOURCE_MANAGER_INCLUDE

#include "resources/Resource.h"
#include "utility/Singleton.h"

namespace sh
{
	
	class SHADOW_API ResourceManager : public Singleton<ResourceManager>
	{
	public:
		ResourceManager();
		~ResourceManager();
		void Init();
	};

} // sh

#endif