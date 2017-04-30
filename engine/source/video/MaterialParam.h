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
		MaterialParamType GetType() const { return m_type; }

		template<typename T>
		void Set(const T& value, u32 arrayIndex = 0U)
		{
			if (!m_dataPtr)
				return;
			T* data = reinterpret_cast<T*>(m_dataPtr) + arrayIndex;
			*data = value;
		}

	//private:
		MaterialParam(MaterialParamType type, u8* dataPtr) : m_type(type), m_dataPtr(dataPtr) { }

	private:
		String m_name;
		MaterialParamType m_type = MaterialParamType::Undefined;
		u8* m_dataPtr = nullptr;
	};

} // video

} // sh

#endif