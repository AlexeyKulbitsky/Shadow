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

		Camera* Camera::Clone()
		{
			Camera* camera = new Camera();

			camera->m_position = m_position;
			camera->m_rotation = m_rotation;

			camera->m_fovy = m_fovy;
			camera->m_nearDistance = m_nearDistance;
			camera->m_farDistance = m_farDistance;
			camera->m_aspectRatio = m_aspectRatio;

			camera->m_projectionMatrix = m_projectionMatrix;
			camera->m_rotationMatrix = m_rotationMatrix;
			camera->m_translationMatrix = m_translationMatrix;
			camera->m_viewMatrix = m_viewMatrix;
			camera->m_viewProjectionMatrix = m_viewProjectionMatrix;
			camera->m_2dProjectionMatrix = m_2dProjectionMatrix;

			camera->m_frontVector = m_frontVector;
			camera->m_upVector = m_upVector;
			camera->m_rightVector = m_rightVector;

			camera->m_needsToRecalculateViewMatrix = m_needsToRecalculateViewMatrix;
			camera->m_needsToRecalculateProjectionMatrix = m_needsToRecalculateProjectionMatrix;

			return camera;
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
			/*auto driverType = Device::GetInstance()->GetDriver()->GetType();
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
			}*/

			SetProjection(fov, width / height, nearPlane, farPlane);
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::BuildRay(u32 x, u32 y, math::Vector3& origin, math::Vector3& direction)
		{
			const auto& viewPort = Device::GetInstance()->GetDriver()->GetViewport();
			
			sh::math::Matrix4 inverseProjMatrix = m_projectionMatrix.GetInversed();
			sh::math::Matrix4 inverseViewMatrix = m_viewMatrix.GetInversed();			
			origin = m_position;
	
			// 3d Normalized Device Coordinates	
			float xNorm = (2.0f * x) / viewPort.GetWidth() - 1.0f;
			float yNorm = 1.0f - (2.0f * y) / viewPort.GetHeight();
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
			// don't forget to normalize the vector at some point
			rayWorld.Normalize();
			direction = rayWorld;
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::SetFov(float fov)
		{
			m_fovy = fov;
			m_needsToRecalculateProjectionMatrix = true;
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::SetFarPlane(float farPlane)
		{
			m_farDistance = farPlane;
			m_needsToRecalculateProjectionMatrix = true;
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::SetNearPlane(float nearPlane)
		{
			m_nearDistance = nearPlane;
			m_needsToRecalculateProjectionMatrix = true;
		}
        
        /////////////////////////////////////////////////////////////////////
        
        void Camera::SetViewport(const math::Rect& viewport)
        {
            m_viewport = viewport;
            m_needsToRecalculateProjectionMatrix = true;
        }

		/////////////////////////////////////////////////////////////////////

		float Camera::GetFov() const 
		{ 
			return m_fovy; 
		}

		/////////////////////////////////////////////////////////////////////

		float Camera::GetNearPlane() const 
		{ 
			return m_nearDistance; 
		}

		/////////////////////////////////////////////////////////////////////

		float Camera::GetFarPlane() const 
		{ 
			return m_farDistance; 
		}
        
        /////////////////////////////////////////////////////////////////////
        
        const math::Rect& Camera::GetViewport() const
        {
            return m_viewport;
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
            
            auto driverType = Device::GetInstance()->GetDriver()->GetType();
            const float width = static_cast<float>(m_viewport.GetWidth());
            const float height = static_cast<float>(m_viewport.GetHeight());
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
