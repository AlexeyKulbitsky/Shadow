#ifndef SHADOW_PROPERTY_INCLUDE
#define SHADOW_PROPERTY_INCLUDE

#include "../Variant.h"
#include "Serializable.h"

#include <type_traits>

namespace sh
{

	class Property
	{
	public:
		Property(const std::string& name) : m_name(name) {}
		virtual void SetValue(Serializable* object, const Variant& value) = 0;
		virtual Variant GetValue(Serializable* object) = 0;
		const std::string& GetName() const;

	private:
		std::string m_name = "";
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename ClassType, typename ReturnType>
	using Getter = ReturnType(ClassType::*)() const;
	template<typename ClassType, typename ArgumentType>
	using Setter = void (ClassType::*)(ArgumentType);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename ClassType, typename FieldType>
	class PropertyImpl : public Property
	{
	public:
		using FieldPtr = FieldType ClassType::*;

		PropertyImpl(FieldPtr ptr, const std::string& name) : Property(name), m_ptr(ptr) {}
		virtual void SetValue(Serializable* object, const Variant& value) override
		{
			ClassType* concreteClass = static_cast<ClassType*>(object);
			concreteClass->*m_ptr = value.Get<FieldType>();
		}
		virtual Variant GetValue(Serializable* object) override
		{
			ClassType* concreteClass = static_cast<ClassType*>(object);
			return Variant(concreteClass->*m_ptr);
		}

	private:
		FieldPtr m_ptr = nullptr;
	};

	template<typename ClassType, typename FieldType>
	Property* CreateFieldProperty(FieldType ClassType::* fieldPtr, const std::string& name)
	{
		return new PropertyImpl<ClassType, FieldType>(fieldPtr, name);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename ClassType, typename ReturnType, typename ArgumentType>
	class AccessorPropertyImpl : public Property
	{
	public:
		using GetterPtr = ReturnType(ClassType::*)() const;
		using SetterPtr = void (ClassType::*) (ArgumentType);

		static_assert(!std::is_pointer<ReturnType>::value && !std::is_pointer<ArgumentType>::value,
			"You can not use pointers to create properties");

		static_assert(std::is_same<std::remove_cv<ReturnType>, std::remove_cv<ArgumentType>>::value,
			"You must use equal types for creating properties");

		using BaseType = std::remove_cv_t<ReturnType>;

		AccessorPropertyImpl(GetterPtr getter, SetterPtr setter, const std::string& name)
			: Property(name)
			, m_getter(getter)
			, m_setter(setter) {}

		virtual void SetValue(Serializable* object, const Variant& value) override
		{
			ClassType* concreteClass = static_cast<ClassType*>(object);
			(concreteClass->*m_setter)(value.Get<BaseType>());
		}
		virtual Variant GetValue(Serializable* object) override
		{
			ClassType* concreteClass = static_cast<ClassType*>(object);
			return Variant((concreteClass->*m_getter)());
		}

	private:
		GetterPtr m_getter;
		SetterPtr m_setter;
	};

	template<typename ClassType, typename ReturnType, typename ArgumentType>
	Property* CreateProperty(Getter<ClassType, ReturnType> getter, Setter<ClassType, ArgumentType> setter, const std::string& name)
	{
		return new AccessorPropertyImpl<ClassType, ReturnType, ArgumentType>(getter, setter, name);
	}

	//////////////////////////////////////////////////////////////

	template<typename ClassType, typename FieldType>
	class EnumPropertyImpl : public Property
	{
	public:
		using FieldPtr = FieldType ClassType::*;

		static_assert(!std::is_pointer<FieldType>::value, "You can not use pointers to create enumeration properties");
		static_assert(std::is_enum<FieldType>::value, "You can not create property for non enumeration types");

		EnumPropertyImpl(FieldPtr ptr, const std::string& name) : Property(name), m_ptr(ptr) {}
		virtual void SetValue(Serializable* object, const Variant& value) override
		{
			ClassType* concreteClass = static_cast<ClassType*>(object);
			concreteClass->*m_ptr = static_cast<FieldType>(value.GetInt());
		}
		virtual Variant GetValue(Serializable* object) override
		{
			ClassType* concreteClass = static_cast<ClassType*>(object);
			return Variant(static_cast<int>(concreteClass->*m_ptr));
		}

	private:
		FieldPtr m_ptr = nullptr;
	};

	template<typename ClassType, typename FieldType>
	Property* CreateEnumProperty(FieldType ClassType::* fieldPtr, const std::string& name)
	{
		return new EnumPropertyImpl<ClassType, FieldType>(fieldPtr, name);
	}

	//////////////////////////////////////////////////////////////

	template<typename ClassType, typename ReturnType, typename ArgumentType>
	class AccessorEnumPropertyImplExt : public Property
	{
	public:
		using GetterPtr = ReturnType(ClassType::*)() const;
		using SetterPtr = void (ClassType::*) (ArgumentType);

		static_assert(!std::is_pointer<ReturnType>::value && !std::is_pointer<ArgumentType>::value,
			"You can not use pointers to create enumeration properties");

		static_assert(std::is_enum<ReturnType>::value && std::is_enum<ArgumentType>::value,
			"You must use Getter/Setters that operate with enumeration types");

		static_assert(std::is_same<std::remove_cv<ReturnType>, std::remove_cv<ArgumentType>>::value,
			"You must use equal types for creating enumeration properties");

		using BaseType = std::remove_cv_t<ReturnType>;

		AccessorEnumPropertyImplExt(GetterPtr getter, SetterPtr setter, const std::string& name)
			: Property(name)
			, m_getter(getter)
			, m_setter(setter) {}

		virtual void SetValue(Serializable* object, const Variant& value) override
		{
			ClassType* concreteClass = static_cast<ClassType*>(object);
			(concreteClass->*m_setter)(static_cast<ArgumentType>(value.GetInt()));
		}
		virtual Variant GetValue(Serializable* object) override
		{
			ClassType* concreteClass = static_cast<ClassType*>(object);
			return Variant(static_cast<int>((concreteClass->*m_getter)()));
		}

	private:
		GetterPtr m_getter;
		SetterPtr m_setter;
	};

	template<typename ClassType, typename ReturnType, typename ArgumentType>
	Property* CreateEnumAccessorProperty(Getter<ClassType, ReturnType> getter, Setter<ClassType, ArgumentType> setter, const std::string& name)
	{
		return new AccessorEnumPropertyImplExt<ClassType, ReturnType, ArgumentType>(getter, setter, name);
	}

} // sh


#define S_FIELD_PROPERTY(name, field) sh::ObjectFactory::GetInstance()->RegisterProperty<ClassType>(sh::CreateFieldProperty(&ClassType::field, name))
#define S_ACCESSOR_PROPERTY(name, Getter, Setter) sh::ObjectFactory::GetInstance()->RegisterProperty<ClassType>(sh::CreateProperty(&ClassType::Getter, &ClassType::Setter, name))
#define S_ENUM_PROPERTY(name, field) sh::ObjectFactory::GetInstance()->RegisterProperty<ClassType>(sh::CreateEnumProperty(&ClassType::field, name))
#define S_ENUM_ACCESSOR_PROPERTY(name, Getter, Setter) sh::ObjectFactory::GetInstance()->RegisterProperty<ClassType>(sh::CreateEnumAccessorProperty(&ClassType::Getter, &ClassType::Setter, name))

#endif
