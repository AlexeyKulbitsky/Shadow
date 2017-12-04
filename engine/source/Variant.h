#ifndef VARIANT_INCLUDE
#define VARIANT_INCLUDE

#include "Globals.h"

namespace sh
{

	enum SHADOW_API VariantType
	{
		VAR_NOTYPE,
		VAR_INT,
		VAR_FLOAT,
		VAR_BOOL,
		VAR_STRING,
		VAR_VECTOR_2_FLOAT,
		VAR_VECTOR_3_FLOAT,
		VAR_VECTOR_4_FLOAT,
		VAR_QUATERNION_FLOAT,
		VAR_SERIALIZABLE,
		VAR_RESOURCE_REF,
		VAR_RESOURCE_REF_LIST,
		VAR_NAMED_RESOURCE_REF,
		VAR_NAMED_RESOURCE_REF_LIST,
		VAR_CUSTOM
	};

	class Serializable;

	class CustomValue
	{

	};

	template<typename T>
	class CustomValueImpl : public CustomValue
	{
	public:
		CustomValueImpl(const T& value) : m_value(value) { }

		T m_value;
	};

	union SHADOW_API VariantValue
	{
		float floatValue;
		int intValue;
		bool boolValue;
		void* ptrValue;
		//math::Vector2f vector2fValue;
		//math::Vector3f vector3fValue;
		//math::Vector4f vector4fValue;
		//math::Quaternionf quaternionfValue;

		VariantValue() {}
		VariantValue(const VariantValue& value) = delete;
		~VariantValue() {}
	};

	class SHADOW_API Variant
	{
	public:
		Variant(int value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(float value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(bool value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(const std::string& value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(const math::Vector2& value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(const math::Vector3& value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(const math::Vector4& value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(const math::Quaternion& value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(Serializable* value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(const ResourceRef& value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(const ResourceRefList& value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(const NamedResourceRefList& value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(const NamedResourceRef& value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(const Variant& other)
		{
		}

		template<typename T>
		Variant(const T& value)
		{
			int a = 0;
			a++;

			m_customValue = new CustomValueImpl<T>(value);
		}

		~Variant()
		{
		}

		Variant& operator=(int value)
		{
			SetType(VAR_INT);
			m_value.intValue = value;
			return *this;
		}

		Variant& operator=(float value)
		{
			SetType(VAR_FLOAT);
			m_value.floatValue = value;
			return *this;
		}

		Variant& operator=(bool value)
		{
			SetType(VAR_BOOL);
			m_value.boolValue = value;
			return *this;
		}

		Variant& operator=(const std::string& value)
		{
			SetType(VAR_STRING);
			*reinterpret_cast<std::string*>(m_value.ptrValue) = value;
			return *this;
		}

		Variant& operator=(const math::Vector2& value)
		{
			SetType(VAR_VECTOR_2_FLOAT);
			*reinterpret_cast<math::Vector2*>(m_value.ptrValue) = value;
			return *this;
		}

		Variant& operator=(const math::Vector3& value)
		{
			SetType(VAR_VECTOR_3_FLOAT);
			*reinterpret_cast<math::Vector3*>(m_value.ptrValue) = value;
			return *this;
		}

		Variant& operator=(const math::Vector4& value)
		{
			SetType(VAR_VECTOR_4_FLOAT);
			*reinterpret_cast<math::Vector4*>(m_value.ptrValue) = value;
			return *this;
		}

		Variant& operator=(const math::Quaternion& value)
		{
			SetType(VAR_QUATERNION_FLOAT);
			*reinterpret_cast<math::Quaternion*>(m_value.ptrValue) = value;
			return *this;
		}

		Variant& operator=(Serializable* value)
		{
			SetType(VAR_SERIALIZABLE);
			m_value.ptrValue = value;
			return *this;
		}

		Variant& operator=(const ResourceRef& value)
		{
			SetType(VAR_RESOURCE_REF);
			*reinterpret_cast<ResourceRef*>(m_value.ptrValue) = value;
			return *this;
		}

		Variant& operator=(const ResourceRefList& value)
		{
			SetType(VAR_RESOURCE_REF_LIST);
			*reinterpret_cast<ResourceRefList*>(m_value.ptrValue) = value;
			return *this;
		}

		Variant& operator=(const NamedResourceRef& value)
		{
			SetType(VAR_NAMED_RESOURCE_REF);
			*reinterpret_cast<NamedResourceRef*>(m_value.ptrValue) = value;
			return *this;
		}

		Variant& operator=(const NamedResourceRefList& value)
		{
			SetType(VAR_NAMED_RESOURCE_REF_LIST);
			*reinterpret_cast<NamedResourceRefList*>(m_value.ptrValue) = value;
			return *this;
		}

		int GetInt() const;
		float GetFloat() const;
		bool GetBool() const;
		const std::string& GetString() const;
		const math::Vector2& GetVector2Float() const;
		const math::Vector3& GetVector3Float() const;
		const math::Vector4& GetVector4Float() const;
		const math::Quaternion& GetQuaternionFloat() const;
		Serializable* GetSerializable() const;
		const ResourceRef& GetResourceRef() const;
		const ResourceRefList& GetResourceRefList() const;
		const NamedResourceRef& GetNamedResourceRef() const;
		const NamedResourceRefList& GetNamedResourceRefList() const;

		template<typename T>
		T Get() const
		{
			return static_cast<CustomValueImpl<T>*>(m_customValue)->m_value;
		}

		VariantType GetType() const { return m_type; }

	private:
		void SetType(VariantType type);

		VariantValue m_value;
		CustomValue* m_customValue = nullptr;
		VariantType m_type = VAR_NOTYPE;
	};

	template <typename T> VariantType GetVariantType();
	template <> inline VariantType GetVariantType<int>() { return VAR_INT; }
	template <> inline VariantType GetVariantType<float>() { return VAR_FLOAT; }
	template <> inline VariantType GetVariantType<bool>() { return VAR_BOOL; }
	template <> inline VariantType GetVariantType<std::string>() { return VAR_STRING; }
	template <> inline VariantType GetVariantType<math::Vector2>() { return VAR_VECTOR_2_FLOAT; }
	template <> inline VariantType GetVariantType<math::Vector3>() { return VAR_VECTOR_3_FLOAT; }
	template <> inline VariantType GetVariantType<math::Vector4>() { return VAR_VECTOR_4_FLOAT; }
	template <> inline VariantType GetVariantType<math::Quaternion>() { return VAR_QUATERNION_FLOAT; }
	template <> inline VariantType GetVariantType<Serializable*>() { return VAR_SERIALIZABLE; }
	template <> inline VariantType GetVariantType<ResourceRef>() { return VAR_RESOURCE_REF; }
	template <> inline VariantType GetVariantType<ResourceRefList>() { return VAR_RESOURCE_REF_LIST; }
	template <> inline VariantType GetVariantType<NamedResourceRef>() { return VAR_NAMED_RESOURCE_REF; }
	template <> inline VariantType GetVariantType<NamedResourceRefList>() { return VAR_NAMED_RESOURCE_REF_LIST; }

	template<> SHADOW_API int Variant::Get<int>() const;
	template<> SHADOW_API float Variant::Get<float>() const;
	template<> SHADOW_API bool Variant::Get<bool>() const;
	template<> SHADOW_API std::string Variant::Get<std::string>() const;
	template<> SHADOW_API math::Vector2 Variant::Get<math::Vector2>() const;
	template<> SHADOW_API math::Vector3 Variant::Get<math::Vector3>() const;
	template<> SHADOW_API math::Vector4 Variant::Get<math::Vector4>() const;
	template<> SHADOW_API math::Quaternion Variant::Get<math::Quaternion>() const;
	template<> SHADOW_API Serializable* Variant::Get<Serializable*>() const;
	template<> SHADOW_API ResourceRef Variant::Get<ResourceRef>() const;
	template<> SHADOW_API ResourceRefList Variant::Get<ResourceRefList>() const;
	template<> SHADOW_API NamedResourceRef Variant::Get<NamedResourceRef>() const;
	template<> SHADOW_API NamedResourceRefList Variant::Get<NamedResourceRefList>() const;

} // sh

#endif
