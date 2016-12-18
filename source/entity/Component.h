#ifndef SHADOW_COMPONENT_INCLUDE
#define SHADOW_COMPONENT_INCLUDE

#include "../Globals.h"

namespace sh
{
	class Component
	{
	public:
		enum class Type
		{
			TRANSFORM = 0,
			RENDER,
			LIGHT,

			COUNT
		};

		static Component* Create(Type type);

		virtual Type GetType() const = 0;
		virtual void Load(const pugi::xml_node &node) = 0;
	};
}

#endif