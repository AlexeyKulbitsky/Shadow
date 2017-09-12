#include "MaterialParam.h"

#include "GpuParams.h"

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

	//////////////////////////////////////////////////////////////////////////////

	MaterialSamplerParam::MaterialSamplerParam(GpuParams* parentParams, const String& name, MaterialParamType type, const u32 set, const u32 binding)
		: m_name(name)
		, m_parentParams(parentParams)
	{
		bool isValid =
			type == MaterialParamType::Sampler2D ||
			type == MaterialParamType::SamplerCube;

		SH_ASSERT(isValid, "Sampler is invalid! Sampler2D will be used instead!");
		if (!isValid)
		{
			m_type = MaterialParamType::Sampler2D;
			return;
		}

		m_type = type;
		m_set = set;
		m_binding = binding;
	}

	SamplerPtr MaterialSamplerParam::GetSampler() const 
	{
		return m_parentParams->GetSampler(m_set, m_binding);
	}

	void MaterialSamplerParam::SetSampler(const SamplerPtr& sampler)
	{
		m_parentParams->SetSampler(sampler, m_set, m_binding);
	}

} // video

} // sh

