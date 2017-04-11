#include "GpuParams.h"

namespace sh
{

namespace video
{
	GpuParams::~GpuParams()
	{
		if (m_data != nullptr)
		{
			delete[] m_data;
			m_data = nullptr;
		}
	}

	GpuParamsPtr GpuParams::Create(const GpuParamsDescription& description)
	{
		GpuParamsPtr result;
		result.reset(new GpuParams(description));
		return result;
	}

	GpuParams::GpuParams(const GpuParamsDescription& desc)
	{
		m_description = desc;

		u32 totalSize = 0U;
		for (auto& param : m_description.params)
		{
			param.second.offset = totalSize;
			totalSize += param.second.size;
		}

		m_data = new u8[totalSize];
	}

} // video

} // sh