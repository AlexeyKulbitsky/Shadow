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
    
    u32 Variant::GetUint() const
    {
        switch (m_type)
        {
            case VAR_UNSIGNED_INT:
                return m_value.uintValue;
                break;
            case VAR_INT:
                return static_cast<u32>(m_value.intValue);
                break;
            case VAR_FLOAT:
                return static_cast<u32>(m_value.floatValue);
                break;
            case VAR_BOOL:
                return static_cast<u32>(m_value.boolValue);
                break;
            default:
                return 0U;
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
		return *reinterpret_cast<std::string*>(m_value.ptrValue);
	}

	const math::Vector2& Variant::GetVector2Float() const
	{
		return *reinterpret_cast<math::Vector2*>(m_value.ptrValue);
	}

	const math::Vector3& Variant::GetVector3Float() const
	{
		return *reinterpret_cast<math::Vector3*>(m_value.ptrValue);
	}

	const math::Vector4& Variant::GetVector4Float() const
	{
		return *reinterpret_cast<math::Vector4*>(m_value.ptrValue);
	}
    
    const math::Vector2Int& Variant::GetVector2Int() const
    {
        return *reinterpret_cast<math::Vector2Int*>(m_value.ptrValue);
    }
    
    const math::Vector3Int& Variant::GetVector3Int() const
    {
        return *reinterpret_cast<math::Vector3Int*>(m_value.ptrValue);
    }
    
    const math::Vector4Int& Variant::GetVector4Int() const
    {
        return *reinterpret_cast<math::Vector4Int*>(m_value.ptrValue);
    }
    
    const math::Rect& Variant::GetRect() const
    {
        return *reinterpret_cast<math::Rect*>(m_value.ptrValue);
    }
    
	const video::Color& Variant::GetColor() const
	{
		return *reinterpret_cast<video::Color*>(m_value.ptrValue);
	}

	const math::Quaternion& Variant::GetQuaternionFloat() const
	{
		return *reinterpret_cast<math::Quaternion*>(m_value.ptrValue);
	}
    
    void* Variant::GetVoidPtr() const
    {
        return m_value.ptrValue;
    }

	Serializable* Variant::GetSerializable() const
	{
		if (m_type == VAR_SERIALIZABLE)
			return reinterpret_cast<Serializable*>(m_value.ptrValue);
		return nullptr;
	}
    
    SPtr<Serializable> Variant::GetSerializableSPtr() const
    {
        if (m_type == VAR_SERIALIZABLE_SPTR)
            return *reinterpret_cast<SPtr<Serializable>*>(m_value.ptrValue);
        return SPtr<Serializable>();
    }

	const ResourceRef& Variant::GetResourceRef() const
	{
		return *reinterpret_cast<ResourceRef*>(m_value.ptrValue);
	}

	const ResourceRefList& Variant::GetResourceRefList() const
	{
		return *reinterpret_cast<ResourceRefList*>(m_value.ptrValue);
	}

	const NamedResourceRef& Variant::GetNamedResourceRef() const
	{
		return *reinterpret_cast<NamedResourceRef*>(m_value.ptrValue);
	}

	const NamedResourceRefList& Variant::GetNamedResourceRefList() const
	{
		return *reinterpret_cast<NamedResourceRefList*>(m_value.ptrValue);
	}

	const ScriptRef& Variant::GetScriptRef() const
	{
		return *reinterpret_cast<ScriptRef*>(m_value.ptrValue);
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
		case VAR_VECTOR_2_FLOAT:
		{
			delete reinterpret_cast<math::Vector2*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
		case VAR_VECTOR_3_FLOAT:
		{
			delete reinterpret_cast<math::Vector3*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
		case VAR_VECTOR_4_FLOAT:
		{
			delete reinterpret_cast<math::Vector4*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
        case VAR_VECTOR_2_INT:
        {
            delete reinterpret_cast<math::Vector2Int*>(m_value.ptrValue);
            m_value.ptrValue = nullptr;
        }
            break;
        case VAR_VECTOR_3_INT:
        {
            delete reinterpret_cast<math::Vector3Int*>(m_value.ptrValue);
            m_value.ptrValue = nullptr;
        }
            break;
        case VAR_VECTOR_4_INT:
        {
            delete reinterpret_cast<math::Vector4Int*>(m_value.ptrValue);
            m_value.ptrValue = nullptr;
        }
            break;
        case VAR_RECT:
        {
            delete reinterpret_cast<math::Rect*>(m_value.ptrValue);
            m_value.ptrValue = nullptr;
        }
            break;
		case VAR_COLOR:
		{
			delete reinterpret_cast<video::Color*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
		break;
		case VAR_QUATERNION_FLOAT:
		{
			delete reinterpret_cast<math::Quaternion*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
        case VAR_SERIALIZABLE_SPTR:
        {
            delete reinterpret_cast<SPtr<Serializable>*>(m_value.ptrValue);
            m_value.ptrValue = nullptr;
        }
            break;
		case VAR_RESOURCE_REF:
		{
			delete reinterpret_cast<ResourceRef*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
		case VAR_RESOURCE_REF_LIST:
		{
			delete reinterpret_cast<ResourceRefList*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
		case VAR_NAMED_RESOURCE_REF_LIST:
		{
			delete reinterpret_cast<NamedResourceRefList*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
		case VAR_SCRIPT_REF:
		{
			delete reinterpret_cast<ScriptRef*>(m_value.ptrValue);
			m_value.ptrValue = nullptr;
		}
			break;
		default:
			break;
		}

		m_type = type;

		switch (m_type)
		{
		case VAR_VECTOR_2_FLOAT:
			m_value.ptrValue = new math::Vector2();
			break;
		case VAR_VECTOR_3_FLOAT:
			m_value.ptrValue = new math::Vector3();
			break;
		case VAR_VECTOR_4_FLOAT:
			m_value.ptrValue = new math::Vector4();
			break;
        case VAR_VECTOR_2_INT:
            m_value.ptrValue = new math::Vector2Int();
            break;
        case VAR_VECTOR_3_INT:
            m_value.ptrValue = new math::Vector3Int();
            break;
        case VAR_VECTOR_4_INT:
            m_value.ptrValue = new math::Vector4Int();
            break;
        case VAR_RECT:
            m_value.ptrValue = new math::Rect();
            break;
		case VAR_COLOR:
			m_value.ptrValue = new video::Color();
			break;
		case VAR_QUATERNION_FLOAT:
			m_value.ptrValue = new math::Quaternion();
			break;
        case VAR_SERIALIZABLE_SPTR:
            m_value.ptrValue = new SPtr<Serializable>();
            break;
		case VAR_STRING:
			m_value.ptrValue = new std::string();
			break;
		case VAR_RESOURCE_REF:
			m_value.ptrValue = new ResourceRef();
			break;
		case VAR_RESOURCE_REF_LIST:
			m_value.ptrValue = new ResourceRefList();
			break;
		case VAR_NAMED_RESOURCE_REF_LIST:
			m_value.ptrValue = new NamedResourceRefList();
			break;
		case VAR_SCRIPT_REF:
			m_value.ptrValue = new ScriptRef();
			break;
		default:
			break;
		}
	}

	template<> int Variant::Get<int>() const { return GetInt(); }
	template<> float Variant::Get<float>() const { return GetFloat(); }
	template<> bool Variant::Get<bool>() const { return GetBool(); }
	template<> std::string Variant::Get<std::string>() const { return GetString(); }
	template<> math::Vector2 Variant::Get<math::Vector2>() const { return GetVector2Float(); }
	template<> math::Vector3 Variant::Get<math::Vector3>() const { return GetVector3Float(); }
	template<> math::Vector4 Variant::Get<math::Vector4>() const { return GetVector4Float(); }
    template<> math::Vector2Int Variant::Get<math::Vector2Int>() const { return GetVector2Int(); }
    template<> math::Vector3Int Variant::Get<math::Vector3Int>() const { return GetVector3Int(); }
    template<> math::Vector4Int Variant::Get<math::Vector4Int>() const { return GetVector4Int(); }
    template<> math::Rect Variant::Get<math::Rect>() const { return GetRect(); }
	template<> video::Color Variant::Get<video::Color>() const { return GetColor(); }
	template<> math::Quaternion Variant::Get<math::Quaternion>() const { return GetQuaternionFloat(); }
    template<> void* Variant::Get<void*>() const { return GetVoidPtr(); }
	template<> Serializable* Variant::Get<Serializable*>() const { return GetSerializable(); }
    template<> SPtr<Serializable> Variant::Get<SPtr<Serializable>>() const { return GetSerializableSPtr(); }
	template<> ResourceRef Variant::Get<ResourceRef>() const { return GetResourceRef(); }
	template<> ResourceRefList Variant::Get<ResourceRefList>() const { return GetResourceRefList(); }
	template<> NamedResourceRef Variant::Get<NamedResourceRef>() const { return GetNamedResourceRef(); }
	template<> NamedResourceRefList Variant::Get<NamedResourceRefList>() const { return GetNamedResourceRefList(); }
	template<> ScriptRef Variant::Get<ScriptRef>() const { return GetScriptRef(); }

} // sh
