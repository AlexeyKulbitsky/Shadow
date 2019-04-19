#ifndef SHADOW_GPU_PARAMS_INCLUDE
#define SHADOW_GPU_PARAMS_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/GpuParamsDescription.h"
#include "video/GpuPipelineParamsInfo.h"
#include "math/math.h"

namespace sh
{

namespace video
{

	class GpuParams;

	// Class for holding one param for fast usage
	template<typename T>
	class TGpuParam
	{
	public:
		TGpuParam();
		TGpuParam(GpuParams* parent, GpuParamDataDescription* desc);
		void Set(const T& value, uint32_t index = 0U) const;
		const T& Get(uint32_t index = 0U) const;
		operator bool() const { return m_valuePtr != nullptr; }

	protected:
		GpuParams* m_parent;
		GpuParamDataDescription* m_desc;
		T* m_valuePtr = nullptr;
	};

	class SHADOW_API TGpuParamStruct
	{
		void Set(const void* value, uint32_t size, uint32_t index = 0U) const;
		void Get(void* value, uint32_t size, uint32_t index = 0U) const;
        uint32_t GetElementSize() const;
	};

    using GpuParamFloat = TGpuParam<float>;
    using GpuParamInt = TGpuParam<int>;
    using GpuParamVector2f = TGpuParam<math::Vector2>;
    using GpuParamVector3f = TGpuParam<math::Vector3>;
    using GpuParamVector4f = TGpuParam<math::Vector4>;
    using GpuParamMatrix3f = TGpuParam<math::Matrix3>;
    using GpuParamMatrix4f = TGpuParam<math::Matrix4>;

	class SHADOW_API GpuParams
	{
	public:
		~GpuParams();

		template<typename T>
		void GetParam(const std::string& name, TGpuParam<T>& param);

		template<typename T>
		void SetParam(const std::string& name, const T& value);

        const uint8_t* GetData() const;

        const std::vector<SamplerPtr>& GetSamplers() const;
		// Sets texture for sampler in specified shader
		void SetSampler(ShaderType shaderType, const std::string& name, const TexturePtr& texture);

		const SamplerPtr GetSampler(const std::string& name) const;
		const SamplerPtr GetSampler(const uint32_t set, const uint32_t binding);

		void SetSampler(const std::string& name, const SamplerPtr& sampler);
		void SetSampler(const std::string& name, const TexturePtr& texture);
		void SetSampler(const SamplerPtr& sampler, const uint32_t set, const uint32_t binding);

        const GpuPipelineParamsInfoPtr& GetParamsInfo() const;

		static GpuParamsPtr Create(const GpuPipelineParamsInfoPtr& pipelineParamsInfo);

	protected:
		GpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo);

	private:
		GpuPipelineParamsInfoPtr m_paramsInfo;

		uint8_t* m_data = nullptr;
		std::vector<SamplerPtr> m_samplers;
	};

	template<typename T>
	inline TGpuParam<T>::TGpuParam()
		: m_parent(nullptr)
		, m_desc(nullptr)
		, m_valuePtr(nullptr)
	{
	}

	template<typename T>
	inline TGpuParam<T>::TGpuParam(GpuParams* parent, GpuParamDataDescription* desc)
		: m_parent(parent)
		, m_desc(desc)
	{
		m_valuePtr = reinterpret_cast<T*>(parent->m_data + desc->offset);
	}

	template<typename T>
	inline void TGpuParam<T>::Set(const T& value, uint32_t index) const
	{
		*(m_valuePtr + index) = value;
	}

	template<typename T>
	inline const T& TGpuParam<T>::Get(uint32_t index) const
	{
		return *(m_valuePtr + index);
	}

	template<typename T>
	inline void GpuParams::GetParam(const std::string& name, TGpuParam<T>& param)
	{

		for( size_t i = 0; i < 6U; ++i )
		{
			const auto& description = m_paramsInfo->GetParamsDescription(ShaderType(i));
			if(!description)
				continue;

			auto it = description->params.find(name);
			if (it != description->params.end())
			{
				param = TGpuParam<T>(this, &(it->second));
				return;
			}
		}		
	}

	template<typename T>
	inline void GpuParams::SetParam(const std::string& name, const T& value)
	{
		TGpuParam<T> param;
		GetParam(name, param);
		param.Set(value);
	}


} // video

} // sh

#endif