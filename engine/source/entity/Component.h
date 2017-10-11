#ifndef SHADOW_COMPONENT_INCLUDE
#define SHADOW_COMPONENT_INCLUDE

#include "../Globals.h"

namespace sh
{
	static const size_t MaxComponents = 32;
	class Entity;

	struct IdCounter
	{
		static size_t counter;
		static size_t GetId() { return counter++; }
	};

#define COMPONENT \
static uintptr_t GetTypeId() { size_t value; return reinterpret_cast<uintptr_t>(&value); }

	class Component
	{
	public:
		enum class Type
		{
			Transform = 0,
			Render,
			Light,
			Terrain,

			Count
		};
		

		static Component* Create(Type type);
		virtual ~Component() {}
		virtual Type GetType() const = 0;
		virtual void Load(const pugi::xml_node &node) = 0;
		virtual void Save(pugi::xml_node &parent) = 0;

		void SetParentEntity(Entity* entity) { m_parentEntity = entity; }
		Entity* GetParentEntity() const { return m_parentEntity; }

		static size_t GetId() { static size_t id = IdCounter::GetId(); return id; }

	protected:
		Bitset<MaxComponents> m_mask;
		Entity* m_parentEntity = nullptr;
	};
}

#endif