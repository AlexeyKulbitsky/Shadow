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
		MaterialParamUsage GetUsage() const { return m_usage; }

		template<typename T>
		void Set(const T& value, u32 arrayIndex = 0U)
		{
			if (!m_dataPtr)
				return;
			T* data = reinterpret_cast<T*>(m_dataPtr) + arrayIndex;
			*data = value;
		}

	//private:
		MaterialParam(MaterialParamUsage usage, u8* dataPtr) : m_usage(usage), m_dataPtr(dataPtr) { }

	private:
		String m_name;
		MaterialParamUsage m_usage = MaterialParamUsage::Undefined;
		u8* m_dataPtr = nullptr;
	};

} // video

} // sh

#endif