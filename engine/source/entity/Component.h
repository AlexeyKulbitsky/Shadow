#ifndef SHADOW_COMPONENT_INCLUDE
#define SHADOW_COMPONENT_INCLUDE

#include "../Globals.h"

namespace sh
{
	class Entity;

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
		virtual ~Component() {}
		virtual Type GetType() const = 0;
		virtual void Load(const pugi::xml_node &node) = 0;
		virtual void Save(pugi::xml_node &parent) = 0;

		void SetParentEntity(Entity* entity) { m_parentEntity = entity; }
		Entity* GetParentEntity() const { return m_parentEntity; }

	protected:
		Entity* m_parentEntity = nullptr;
	};
}

#endif