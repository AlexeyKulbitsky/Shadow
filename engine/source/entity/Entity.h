#ifndef SHADOW_ENTITY_INCLUDE
#define SHADOW_ENTITY_INCLUDE

#include "../Globals.h"
#include "Component.h"

namespace sh
{
	class Entity
	{
	public:
		Entity();
		~Entity();

		void Load(const pugi::xml_node& parent);
		void Save(pugi::xml_node& parent);
		const String& GetName() const { return m_name; }
		void SetName(const String& name) { m_name = name; }
		void AddComponent(Component* component);

		template<typename T>
		T* GetComponent()
		{
			const auto id = T::GetTypeId();
			if (id >= m_components.size())
				return nullptr;
			return static_cast<T*>(m_components[id]);
		}

		bool IntersectsRay(const math::Vector3f& origin, const math::Vector3f& direction);

	private:	
		String m_name = "unnamed";

		std::vector<Component*> m_components;
	};
}

#endif