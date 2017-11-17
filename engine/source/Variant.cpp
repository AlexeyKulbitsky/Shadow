#include "Variant.h"

namespace sh
{

	int Variant::GetInt() const
	{
		switch (m_type)
		{
		case VAR_INT:
			return m_int;
			break;
		case VAR_FLOAT:
			return static_cast<int>(m_float);
			break;
		case VAR_BOOL:
			return static_cast<int>(m_bool);
			break;
		default:
			return 0;
		}
	}

	float Variant::GetFloat() const
	{
		switch (m_type)
		{
		case VAR_INT:
			return static_cast<float>(m_int);
			break;
		case VAR_FLOAT:
			return m_float;
			break;
		case VAR_BOOL:
			return static_cast<float>(m_bool);
			break;
		default:
			return 0.0f;
		}
	}

	bool Variant::GetBool() const
	{
		switch (m_type)
		{
		case VAR_INT:
			return static_cast<bool>(m_int);
			break;
		case VAR_FLOAT:
			return static_cast<bool>(m_float);
			break;
		case VAR_BOOL:
			return m_bool;
			break;
		default:
			return false;
		}
	}

	const std::string& Variant::GetString() const
	{
		if (m_type == VAR_STRING)
			return *reinterpret_cast<std::string*>(m_ptr);
		return std::string();
	}

	const math::Vector3f& Variant::GetVector3Float() const
	{
		return m_vector3f;
	}

	Serializable* Variant::GetSerializable() const
	{
		if (m_type == VAR_SERIALIZABLE)
			return reinterpret_cast<Serializable*>(m_ptr);
		return nullptr;
	}

	void Variant::SetType(VariantType type)
	{
		switch (m_type)
		{
		case VAR_STRING:
			delete reinterpret_cast<std::string*>(m_ptr);
			break;
		default:
			break;
		}

		m_type = type;

		switch (m_type)
		{
		case VAR_STRING:
			m_ptr = new std::string();
			break;
		default:
			break;
		}
	}

	template<> int Variant::Get<int>() const { return GetInt(); }
	template<> float Variant::Get<float>() const { return GetFloat(); }
	template<> bool Variant::Get<bool>() const { return GetBool(); }
	template<> std::string Variant::Get<std::string>() const { return GetString(); }
	template<> math::Vector3f Variant::Get<math::Vector3f>() const { return GetVector3Float(); }
	template<> Serializable* Variant::Get<Serializable*>() const { return GetSerializable(); }

} // sh
