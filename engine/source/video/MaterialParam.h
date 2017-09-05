#ifndef SHADOW_MATERIAL_PARAM_INCLUDE
#define SHADOW_MATERIAL_PARAM_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace video
{
	
	class MaterialParam
	{
		friend class MaterialParams;
		friend class Material;
	public:
		MaterialParam(const String& name, MaterialParamType type, MaterialParamUsage usage, u8* dataPtr);
		MaterialParam(const String& name, MaterialParamType type, u8* dataPtr);

		const String& GetName() const { return m_name; }
		MaterialParamUsage GetUsage() const { return m_usage; }
		MaterialParamType GetType() const { return m_type; }

		template<typename T>
		void Set(const T& value, u32 arrayIndex = 0U)
		{
			if (!m_dataPtr)
				return;
			T* data = reinterpret_cast<T*>(m_dataPtr) + arrayIndex;
			*data = value;
		}

		template<typename T>
		void Get(T& value, u32 arrayIndex = 0U)
		{
			if (!m_dataPtr)
				return;
			T* data = reinterpret_cast<T*>(m_dataPtr) + arrayIndex;
			value = *data;
		}

	private:
		String m_name;
		MaterialParamUsage m_usage = MaterialParamUsage::Undefined;
		MaterialParamType m_type;
		u8* m_dataPtr = nullptr;
	};

} // video

} // sh

#endif