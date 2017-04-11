#ifndef SHADOW_GPU_PARAMS_INCLUDE
#define SHADOW_GPU_PARAMS_INCLUDE

#include "../Globals.h"
#include "GpuParam.h"
#include "GpuParamsDescription.h"

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

	class TGpuParamStruct
	{
		void Set(const void* value, u32 size, u32 index = 0U) const;
		void Get(void* value, u32 size, u32 index = 0U) const;
		u32 GetElementSize() const;
	};

	typedef TGpuParam<float> GpuParamFloat;
	typedef TGpuParam<int> GpuParamInt;
	typedef TGpuParam<math::Vector2f> GpuParamVector2f;
	typedef TGpuParam<math::Vector3f> GpuParamVector3f;
	typedef TGpuParam<math::Vector4f> GpuParamVector4f;
	typedef TGpuParam<math::Matrix3f> GpuParamMatrix3f;
	typedef TGpuParam<math::Matrix4f> GpuParamMatrix4f;

	///////////////////////////////////////////////////////////////


	class GpuParams
	{
		template<typename U>
		friend class TGpuParam;
	public:
		~GpuParams();

		template<typename T>
		void GetParam(const String& name, TGpuParam<T>& param);

		const u8* GetData() const { return m_data; }
		const GpuParamsDescription& GetDescripton() const { return m_description; }

		static GpuParamsPtr Create(const GpuParamsDescription& description);
	private:
		GpuParams(const GpuParamsDescription& description);
		
	private:
		GpuParamsDescription m_description;

		std::array<GpuParamsDescription, 6U> m_paramsDescriptions;
		u8* m_data = nullptr;
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
		auto it = m_description.params.find(name);
		if (it != m_description.params.end())
		{
			param = TGpuParam<T>(this, &(it->second));
		}
	}


} // video

} // sh

#endif