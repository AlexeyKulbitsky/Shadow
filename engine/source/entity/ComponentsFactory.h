#ifndef SHADOW_COMPONENTS_FACTORY_INCLUDE
#define SHADOW_COMPONENTS_FACTORY_INCLUDE

#include "Component.h"

namespace sh
{
	class SHADOW_API ComponentsFactory
	{
	public:
		virtual ~ComponentsFactory();

	protected:
		std::vector<Component*> m_components;
	};
}

#endif