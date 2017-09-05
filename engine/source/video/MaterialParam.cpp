#include "MaterialParam.h"

namespace sh
{

namespace video
{

	MaterialParam::MaterialParam(const String& name, MaterialParamType type, MaterialParamUsage usage, u8* dataPtr)
		: m_name(name)
		, m_usage(usage)
		, m_type(type)
		, m_dataPtr(dataPtr) 
	{ 
	}

	MaterialParam::MaterialParam(const String& name, MaterialParamType type, u8* dataPtr)
		: m_name(name)
		, m_type(type)
		, m_dataPtr(dataPtr)
	{
	}

} // video

} // sh

