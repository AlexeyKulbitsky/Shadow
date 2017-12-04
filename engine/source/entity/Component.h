#ifndef SHADOW_COMPONENT_INCLUDE
#define SHADOW_COMPONENT_INCLUDE

#include "../Globals.h"
#include "../serialization/Serializable.h"

namespace sh
{
	static const size_t MaxComponents = 32;
	class Entity;

	struct SHADOW_API ComponentTypeIdGenerator
	{
		static size_t& currentId()
		{
			static size_t id = 0U;
			return id;
		}
		static size_t GetAvailableId() { return currentId()++; }
	};

#define COMPONENT(typeName) \
SHADOW_OBJECT(typeName) \
static size_t GetTypeId() { static size_t id = ComponentTypeIdGenerator::GetAvailableId(); return id; } \
virtual size_t GetId() override { return GetTypeId(); }

	class SHADOW_API Component : public Serializable
	{
	public:
		virtual ~Component() {}
		virtual size_t GetId() = 0;

		void SetParentEntity(Entity* entity) { m_parentEntity = entity; }
		Entity* GetParentEntity() const { return m_parentEntity; }

	protected:
		Bitset<MaxComponents> m_mask;
		Entity* m_parentEntity = nullptr;
	};
}

#endif