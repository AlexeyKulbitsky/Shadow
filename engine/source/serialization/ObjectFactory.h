#ifndef SHADOW_OBJECT_FACTORY
#define SHADOW_OBJECT_FACTORY

#include "Property.h"

namespace sh
{

	class ConcreteObjectFactory
	{
	public:
		virtual Serializable* CreateObject() = 0;
		const std::string& GetName() { return m_name; }

	protected:
		std::string m_name;
	};

	template<typename T>
	class ConcreteObjectFactoryImpl : public ConcreteObjectFactory
	{
	public:
		ConcreteObjectFactoryImpl()
		{
			m_name = T::GetTypeNameStatic();
		}
		virtual Serializable* CreateObject() override { return new T(); }
	};

	class SHADOW_API ObjectFactory : public Singleton<ObjectFactory>
	{
	public:

		template<typename T>
		void RegisterFactory()
		{
			ConcreteObjectFactory* factory = new ConcreteObjectFactoryImpl<T>();
			m_factories[factory->GetName()] = factory;
		}

		template<typename T>
		void RegisterFactory(const String& group)
		{
			RegisterFactory<T>();
			m_objectGroups[group].push_back(T::GetTypeNameStatic());
		}

		template<typename T>
		void RegisterProperty(Property* property)
		{
			m_propertes[T::GetTypeNameStatic()][property->GetName()] = property;
		}
        
        template<typename T, typename Parent>
        void RegisterParentProperties()
        {
            static_assert(std::is_base_of<Parent, T>::value, "T is not an inheritant of Base!");
            auto properties = GetProperties<Parent>();
            for (auto property : (*properties))
            {
                m_propertes[T::GetTypeNameStatic()][property.second->GetName()] = property.second;
            }
        }

		Property* GetProperty(const std::string& objectTypeName, const std::string& name);
		template<typename T>
		Property* GetProperty(const std::string& name)
		{
			return GetProperty(T::GetTypeNameStatic(), name);
		}

		std::map<std::string, Property*>* GetProperties(const std::string& objectTypeName);
		template<typename T>
		std::map<std::string, Property*>* GetProperties()
		{
			return GetProperties(T::GetTypeNameStatic());
		}

		Serializable* CreateObject(const std::string& type);
		std::vector<std::string> GetObjectTypes() const;
		std::vector<std::string> GetObjectTypesForGroup(const String& groupName) const;
        String GetGroupForObject(Serializable* object);

	private:
		std::map<std::string, ConcreteObjectFactory*> m_factories;
		std::map<std::string, std::vector<String>> m_objectGroups;
		std::map<std::string, std::map<std::string, Property*>> m_propertes;
	};

} // sh

#endif
