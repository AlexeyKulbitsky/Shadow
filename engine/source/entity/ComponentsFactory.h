#ifndef SHADOW_COMPONENTS_FACTORY_INCLUDE
#define SHADOW_COMPONENTS_FACTORY_INCLUDE

#include "Component.h"

namespace sh
{
	class ComponentsFactory
	{
	public:
		virtual ~ComponentsFactory();

	protected:
		std::vector<Component*> m_components;
	};
}

#endif