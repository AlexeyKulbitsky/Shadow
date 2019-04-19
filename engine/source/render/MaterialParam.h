#ifndef SHADOW_MATERIAL_PARAM_INCLUDE
#define SHADOW_MATERIAL_PARAM_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/Common.h"

namespace sh
{

namespace video
{

	struct SHADOW_API MaterialParamDescription
	{
		std::string name;
        std::string description;
		MaterialParamType type;
	};

    class SHADOW_API MaterialParamBase
    {
    public:
        MaterialParamBase(const std::string& name, MaterialParamType type, bool isAutoParam);

        const std::string& GetName() const;
        MaterialParamType GetType() const;
        const bool IsAutoParam() const;

    protected:
        std::string m_name;
        MaterialParamType m_type;
        bool m_isAutoParam = false;
    };

	class SHADOW_API MaterialParam : public MaterialParamBase
	{
	public:
		MaterialParam(const std::string& name, MaterialParamType type, MaterialParamUsage usage, uint8_t* dataPtr, bool isAutoParam = false);
		MaterialParam(const std::string& name, MaterialParamType type, uint8_t* dataPtr, bool isAutoParam = false);

        MaterialParamUsage GetUsage() const;

		template<typename T>
		void Set(const T& value, uint32_t arrayIndex = 0U)
		{
			if (!m_dataPtr)
				return;
			T* data = reinterpret_cast<T*>(m_dataPtr) + arrayIndex;
			*data = value;
		}

		template<typename T>
		void Get(T& value, uint32_t arrayIndex = 0U) const
		{
			if (!m_dataPtr)
				return;
			T* data = reinterpret_cast<T*>(m_dataPtr) + arrayIndex;
			value = *data;
		}

	private:
        std::string m_name;
		MaterialParamUsage m_usage = MaterialParamUsage::Undefined;
		MaterialParamType m_type;
        uint8_t* m_dataPtr = nullptr;
	};

	class SHADOW_API MaterialSamplerParam : public MaterialParamBase
	{
	public:
		MaterialSamplerParam(GpuParams* parentParams, const std::string& name, MaterialParamType type, const uint32_t set, const uint32_t binding, bool isAutoParam = false);

		SamplerPtr GetSampler() const;
		void SetSampler(const SamplerPtr& sampler);

	private:
		uint32_t m_set = 0U;
		uint32_t m_binding = 0U;
		GpuParams* m_parentParams = nullptr;
	};

} // video

} // sh

#endif