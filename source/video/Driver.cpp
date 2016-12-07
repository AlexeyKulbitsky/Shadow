#include "Driver.h"

namespace sh
{
	namespace video
	{
		template<typename T>
		void Driver::SetGlobalUniform(GlobalUniformName globalName, const T& value)
		{
			m_globalUniforms[(size_t)globalName]->Set(value);
		}

		void Driver::InitGlobalUniforms()
		{
			m_globalUniforms[(size_t)GlobalUniformName::MODEL_WORLD_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::MODEL_NORMAL_MATRIX] = new Uniform(math::Matrix3f());

			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_VIEW_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_VIEW_TRANSLATION_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_VIEW_ROTATION_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_PROJECTION_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_VIEW_PROJECTION_MATRIX] = new Uniform(math::Matrix4f());
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_VIEW_ROTATION_PROJECTION_MATRIX] = new Uniform(math::Matrix4f());
						
			m_globalUniforms[(size_t)GlobalUniformName::CAMERA_POSITION] = new Uniform(math::Vector3f());
		}
	}
}