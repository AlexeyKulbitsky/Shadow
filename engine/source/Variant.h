#ifndef VARIANT_INCLUDE
#define VARIANT_INCLUDE

#include "Globals.h"

namespace sh
{

	enum VariantType
	{
		VAR_NOTYPE,
		VAR_INT,
		VAR_FLOAT,
		VAR_BOOL,
		VAR_STRING,
		VAR_VECTOR3FLOAT,
		VAR_SERIALIZABLE,
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

	class Variant
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

		Variant(const math::Vector3f& value)
			: m_type(VAR_NOTYPE)
		{
			*this = value;
		}

		Variant(Serializable* value)
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

		Variant& operator=(int value)
		{
			SetType(VAR_INT);
			m_int = value;
			return *this;
		}

		Variant& operator=(float value)
		{
			SetType(VAR_FLOAT);
			m_float = value;
			return *this;
		}

		Variant& operator=(bool value)
		{
			SetType(VAR_BOOL);
			m_bool = value;
			return *this;
		}

		Variant& operator=(const std::string& value)
		{
			SetType(VAR_STRING);
			*reinterpret_cast<std::string*>(m_ptr) = value;
			return *this;
		}

		Variant& operator=(const math::Vector3f& value)
		{
			SetType(VAR_VECTOR3FLOAT);
			m_vector3f = value;
			return *this;
		}

		Variant& operator=(Serializable* value)
		{
			SetType(VAR_SERIALIZABLE);
			m_ptr = value;
			return *this;
		}

		int GetInt() const;
		float GetFloat() const;
		bool GetBool() const;
		const std::string& GetString() const;
		const math::Vector3f& GetVector3Float() const;
		Serializable* GetSerializable() const;

		template<typename T>
		T Get() const
		{
			return static_cast<CustomValueImpl<T>*>(m_customValue)->m_value;
		}

		VariantType GetType() const { return m_type; }

	private:
		void SetType(VariantType type);

		union
		{
			float m_float;
			int m_int;
			bool m_bool;
			void* m_ptr;
			math::Vector3f m_vector3f;
		};
		CustomValue* m_customValue = nullptr;
		VariantType m_type = VAR_NOTYPE;
	};

	template<> int Variant::Get<int>() const;
	template<> float Variant::Get<float>() const;
	template<> bool Variant::Get<bool>() const;
	template<> std::string Variant::Get<std::string>() const;
	template<> math::Vector3f Variant::Get<math::Vector3f>() const;
	template<> Serializable* Variant::Get<Serializable*>() const;

} // sh

#endif
