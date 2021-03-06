#ifndef SHADOW_GPU_PARAMS_INCLUDE
#define SHADOW_GPU_PARAMS_INCLUDE

#include "../Globals.h"
#include "GpuParam.h"
#include "GpuParamsDescription.h"
#include "GpuPipelineParamsInfo.h"

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
		void Set(const T& value, u32 index = 0U) const;
		const T& Get(u32 index = 0U) const;
		operator bool() const { return m_valuePtr != nullptr; }

	protected:
		GpuParams* m_parent;
		GpuParamDataDescription* m_desc;
		T* m_valuePtr = nullptr;
	};

	class SHADOW_API TGpuParamStruct
	{
		void Set(const void* value, u32 size, u32 index = 0U) const;
		void Get(void* value, u32 size, u32 index = 0U) const;
		u32 GetElementSize() const;
	};

	typedef TGpuParam<float> GpuParamFloat;
	typedef TGpuParam<int> GpuParamInt;
	typedef TGpuParam<math::Vector2> GpuParamVector2f;
	typedef TGpuParam<math::Vector3> GpuParamVector3f;
	typedef TGpuParam<math::Vector4> GpuParamVector4f;
	typedef TGpuParam<math::Matrix3> GpuParamMatrix3f;
	typedef TGpuParam<math::Matrix4> GpuParamMatrix4f;

	///////////////////////////////////////////////////////////////


	class SHADOW_API GpuParams
	{
		friend class HardwareBufferManager;
		template<typename U>
		friend class TGpuParam;
	public:
		~GpuParams();

		template<typename T>
		void GetParam(const String& name, TGpuParam<T>& param);

		template<typename T>
		void SetParam(const String& name, const T& value);

		const u8* GetData() const { return m_data; }

		const std::vector<SamplerPtr>& GetSamplers() const { return m_samplers; }
		// Sets texture for sampler in specified shader
		void SetSampler(ShaderType shaderType, const String& name, const TexturePtr& texture);

		const SamplerPtr GetSampler(const String& name) const;
		const SamplerPtr GetSampler(const u32 set, const u32 binding);

		void SetSampler(const String& name, const SamplerPtr& sampler);
		void SetSampler(const String& name, const TexturePtr& texture);
		void SetSampler(const SamplerPtr& sampler, const u32 set, const u32 binding);

		const GpuPipelineParamsInfoPtr& GetParamsInfo() const { return m_paramsInfo; }

		static GpuParamsPtr Create(const GpuPipelineParamsInfoPtr& pipelineParamsInfo);

	protected:
		GpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo);

	private:
		GpuPipelineParamsInfoPtr m_paramsInfo;

		u8* m_data = nullptr;
		std::vector<SamplerPtr> m_samplers;
	};

	///////////////////////////////////////////////////////

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
	inline void TGpuParam<T>::Set(const T& value, u32 index) const
	{
		*(m_valuePtr + index) = value;
	}

	template<typename T>
	inline const T& TGpuParam<T>::Get(u32 index) const
	{
		return *(m_valuePtr + index);
	}

	template<typename T>
	inline void GpuParams::GetParam(const String& name, TGpuParam<T>& param)
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
	inline void GpuParams::SetParam(const String& name, const T& value)
	{
		TGpuParam<T> param;
		GetParam(name, param);
		param.Set(value);
	}


} // video

} // sh

#endif