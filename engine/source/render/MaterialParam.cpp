#include "render/MaterialParam.h"
#include "video/GpuParams.h"

namespace sh
{

namespace video
{

    MaterialParamBase::MaterialParamBase(const std::string& name, MaterialParamType type, bool isAutoParam)
        : m_name(name)
        , m_type(type)
        , m_isAutoParam(isAutoParam)
    {
    }

    const std::string& MaterialParamBase::GetName() const
    {
        return m_name;
    }

    MaterialParamType MaterialParamBase::GetType() const
    {
        return m_type;
    }

    const bool MaterialParamBase::IsAutoParam() const
    {
        return m_isAutoParam;
    }

	MaterialParam::MaterialParam(const std::string& name, MaterialParamType type, MaterialParamUsage usage, uint8_t* dataPtr, bool isAutoParam/* = false*/)
        : MaterialParamBase(name, type, isAutoParam)
		, m_usage(usage)
		, m_dataPtr(dataPtr) 
	{ 
	}

	MaterialParam::MaterialParam(const std::string& name, MaterialParamType type, uint8_t* dataPtr, bool isAutoParam/* = false*/)
        : MaterialParamBase(name, type, isAutoParam)
		, m_dataPtr(dataPtr)
	{
	}

    MaterialParamUsage MaterialParam::GetUsage() const 
    { 
        return m_usage; 
    }

	MaterialSamplerParam::MaterialSamplerParam(GpuParams* parentParams, const std::string& name, MaterialParamType type, const uint32_t set, const uint32_t binding, bool isAutoParam/* = false*/)
		: MaterialParamBase(name, type, isAutoParam)
		, m_parentParams(parentParams)
	{
		bool isValid =
			type == MaterialParamType::Sampler2D ||
			type == MaterialParamType::SamplerCube;

		assert(isValid && "Sampler is invalid! Sampler2D will be used instead!");
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

