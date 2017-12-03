#include "Camera.h"
#include "SceneManager.h"
#include "../Device.h"
#include "../video/Driver.h"

namespace sh
{
	namespace scene
	{
		Camera::Camera()
		{
			m_frontVector = SceneManager::GetFrontVector();
			m_rightVector = SceneManager::GetRightVector();
			m_upVector = SceneManager::GetUpVector();
			m_rotation.SetIndentity();
		}

		/////////////////////////////////////////////////////////////////////

		Camera::~Camera()
		{

		}

		/////////////////////////////////////////////////////////////////////

		void Camera::Update()
		{
			if (m_needsToRecalculateViewMatrix)
			{
				UpdateViewMatrix();
				m_needsToRecalculateViewMatrix = false;
			}
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::SetPosition(const math::Vector3& pos)
		{
			m_position = pos;
			m_needsToRecalculateViewMatrix = true;
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::SetRotation(const math::Quaternion& rot)
		{
			m_rotation = rot;
			m_needsToRecalculateViewMatrix = true;

			m_upVector = m_rotation * SceneManager::GetUpVector();
			m_frontVector = m_rotation * SceneManager::GetFrontVector();
			m_rightVector = m_rotation * SceneManager::GetRightVector();
			m_rightVector.Normalize();
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::SetProjection(f32 fov, f32 aspect, f32 nearPlane, f32 farPlane)
		{
			m_fovy = fov;
			m_aspectRatio = aspect;
			m_nearDistance = nearPlane;
			m_farDistance = farPlane;

			UpdateProjectionMatrix();
			m_needsToRecalculateProjectionMatrix = false;
		}

		void Camera::SetProjection(f32 fov, f32 width, f32 height, f32 nearPlane, f32 farPlane)
		{
			auto driverType = Device::GetInstance()->GetDriver()->GetType();
			switch (driverType)
			{
				case video::DriverType::OPENGL_ES_2_0:
					m_2dProjectionMatrix.SetOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
					break;
#if defined(USE_VULKAN_API)
				case video::DriverType::VULKAN:
					m_2dProjectionMatrix.SetOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
					break;
#endif
				default:
					break;
			}

			SetProjection(fov, width / height, nearPlane, farPlane);
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::BuildRay(u32 x, u32 y, math::Vector3& origin, math::Vector3& direction)
		{
			math::Vector4 viewPort = Device::GetInstance()->GetDriver()->GetViewPort();
			
			sh::math::Matrix4 inverseProjMatrix = m_projectionMatrix.GetInversed();
			sh::math::Matrix4 inverseViewMatrix = m_viewMatrix.GetInversed();			
			origin = m_position;
	
			// 3d Normalised Device Coordinates	
			float xNorm = (2.0f * x) / viewPort.z - 1.0f;
			float yNorm = 1.0f - (2.0f * y) / viewPort.w;
			float zNorm = 1.0f;
			sh::math::Vector3 rayNds(xNorm, yNorm, zNorm);

			// 4d Homogeneous Clip Coordinates
			sh::math::Vector4 rayClip(rayNds.x, rayNds.y, -1.0f, 1.0f);

			// 4d Eye (Camera) Coordinates
			sh::math::Vector4 rayEye = inverseProjMatrix * rayClip;
			rayEye = sh::math::Vector4(rayEye.x, rayEye.y, -1.0f, 0.0f);

			// 4d World Coordinates
			sh::math::Vector4 temp = inverseViewMatrix * rayEye;
			sh::math::Vector3 rayWorld(temp.x, temp.y, temp.z);
			// don't forget to normalise the vector at some point
			rayWorld.Normalize();
			direction = rayWorld;
		}

		/////////////////////////////////////////////////////////////////////

		const math::Matrix4& Camera::GetViewMatrix()
		{
			if (m_needsToRecalculateViewMatrix)
			{
				UpdateViewMatrix();				
				m_needsToRecalculateViewMatrix = false;
				m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
			}
			return m_viewMatrix;
		}

		/////////////////////////////////////////////////////////////////////

		const math::Matrix4& Camera::GetProjectionMatrix()
		{
			if (m_needsToRecalculateProjectionMatrix)
			{
				UpdateProjectionMatrix();
				m_needsToRecalculateProjectionMatrix = false;

				m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
			}
			return m_projectionMatrix;
		}

		/////////////////////////////////////////////////////////////////////

		const math::Matrix4& Camera::GetViewProjectionMatrix()
		{
			if (m_needsToRecalculateProjectionMatrix)
			{
				UpdateProjectionMatrix();
				m_needsToRecalculateProjectionMatrix = false;
				m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
			}
			if (m_needsToRecalculateViewMatrix)
			{
				UpdateViewMatrix();
				m_needsToRecalculateViewMatrix = false;
				m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
			}
			
			return m_viewProjectionMatrix;
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::UpdateViewMatrix()
		{
			// Update rotation part
			m_rotationMatrix.SetIdentity();
			m_rotationMatrix = m_rotation.GetInverted().GetAsMatrix4();

			// Update translation part
			m_translationMatrix.SetIdentity();
			m_translationMatrix.SetTranslation(-m_position);

			m_viewMatrix = m_rotationMatrix * m_translationMatrix;
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::UpdateProjectionMatrix()
		{
			m_projectionMatrix.SetPerspective(m_fovy, m_aspectRatio, m_nearDistance, m_farDistance);
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::UpdateMatrixTransform()
		{
			if (m_needsToRecalculateViewMatrix)
				UpdateViewMatrix();

			if (m_needsToRecalculateProjectionMatrix)
				UpdateProjectionMatrix();
		}

		/////////////////////////////////////////////////////////////////////

	}
}