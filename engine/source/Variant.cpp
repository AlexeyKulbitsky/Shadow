#include "Variant.h"

namespace sh
{

	int Variant::GetInt() const
	{
		switch (m_type)
		{
		case VAR_INT:
			return m_value.intValue;
			break;
		case VAR_FLOAT:
			return static_cast<int>(m_value.floatValue);
			break;
		case VAR_BOOL:
			return static_cast<int>(m_value.boolValue);
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
			return static_cast<float>(m_value.intValue);
			break;
		case VAR_FLOAT:
			return m_value.floatValue;
			break;
		case VAR_BOOL:
			return static_cast<float>(m_value.boolValue);
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
			return static_cast<bool>(m_value.intValue);
			break;
		case VAR_FLOAT:
			return static_cast<bool>(m_value.floatValue);
			break;
		case VAR_BOOL:
			return m_value.boolValue;
			break;
		default:
			return false;
		}
	}

	const std::string& Variant::GetString() const
	{
		if (m_type == VAR_STRING)
			return *reinterpret_cast<std::string*>(m_value.ptrValue);
		return std::string();
	}

	const math::Vector3f& Variant::GetVector3Float() const
	{
		return m_value.vector3fValue;
	}

	const math::Quaternionf& Variant::GetQuaternionFloat() const
	{
		return m_value.quaternionfValue;
	}

	Serializable* Variant::GetSerializable() const
	{
		if (m_type == VAR_SERIALIZABLE)
			return reinterpret_cast<Serializable*>(m_value.ptrValue);
		return nullptr;
	}

	const ResourceRef& Variant::GetResourceRef() const
	{
		return *reinterpret_cast<ResourceRef*>(m_value.ptrValue);
	}

	const NamedResourceRefList& Variant::GetNamedResourceRefList() const
	{
		return *reinterpret_cast<NamedResourceRefList*>(m_value.ptrValue);
	}

	void Variant::SetType(VariantType type)
	{
		switch (m_type)
		{
		case VAR_STRING:
		{
			delete reinterpret_cast<std::string*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
		case VAR_RESOURCE_REF:
		{
			delete reinterpret_cast<ResourceRef*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
		case VAR_NAMED_RESOURCE_REF_LIST:
		{
			delete reinterpret_cast<NamedResourceRefList*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
		default:
			break;
		}

		m_type = type;

		switch (m_type)
		{
		case VAR_STRING:
			m_value.ptrValue = new std::string();
			break;
		case VAR_RESOURCE_REF:
			m_value.ptrValue = new ResourceRef();
			break;
		case VAR_NAMED_RESOURCE_REF_LIST:
			m_value.ptrValue = new NamedResourceRefList();
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
	template<> math::Quaternionf Variant::Get<math::Quaternionf>() const { return GetQuaternionFloat(); }
	template<> Serializable* Variant::Get<Serializable*>() const { return GetSerializable(); }
	template<> ResourceRef Variant::Get<ResourceRef>() const { return GetResourceRef(); }
	template<> NamedResourceRefList Variant::Get<NamedResourceRefList>() const { return GetNamedResourceRefList(); }

} // sh
