#include "Camera.h"
#include "SceneManager.h"
#include "../Device.h"

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
			InputManager* inputManager = Device::GetInstance()->GetInputManager();

			if (inputManager->IsKeyPressed(KeyCode::KEY_KEY_W))
			{
				SetPosition(m_position + m_frontVector * 0.3f);
			}

			if (inputManager->IsKeyPressed(KeyCode::KEY_KEY_S))
			{
				SetPosition(m_position - m_frontVector * 0.3f);
			}

			if (inputManager->IsKeyPressed(KeyCode::KEY_KEY_A))
			{
				SetPosition(m_position - m_rightVector * 0.3f);
			}

			if (inputManager->IsKeyPressed(KeyCode::KEY_KEY_D))
			{
				SetPosition(m_position + m_rightVector * 0.3f);
			}

			math::Vector2i old = inputManager->GetMousePositionOld();
			math::Vector2i current = inputManager->GetMousePositionCurrent();
			if (inputManager->IsMouseButtonPressed(MouseCode::BUTTON_RIGHT))
			{
				
				math::Vector2i delta = current - old;
				float xAngle = (float)delta.x * 0.01f;
				float yAngle = (float)delta.y * 0.01f;

				math::Quaternionf xRot;
				xRot.SetFromAxisAngle(SceneManager::GetUpVector(), xAngle);

				math::Quaternionf yRot;
				yRot.SetFromAxisAngle(m_rightVector, yAngle);

				SetRotation(xRot * yRot * m_rotation);
			}
			//inputManager->SetMousePositionOld(current);

			if (m_needsToRecalculateViewMatrix)
			{
				UpdateViewMatrix();
				m_needsToRecalculateViewMatrix = false;
			}
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::SetPosition(const math::Vector3f& pos)
		{
			m_position = pos;
			m_needsToRecalculateViewMatrix = true;
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::SetRotation(const math::Quaternionf& rot)
		{
			m_rotation = rot;
			m_needsToRecalculateViewMatrix = true;

			m_upVector = m_rotation * SceneManager::GetUpVector();
			m_frontVector = m_rotation * SceneManager::GetFrontVector();
			m_rightVector = m_rotation * SceneManager::GetRightVector();
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::SetProjection(f32 fov, f32 aspect, f32 nearP, f32 farP)
		{
			m_fovy = fov;
			m_aspectRatio = aspect;
			m_nearDistance = nearP;
			m_farDistance = farP;

			UpdateProjectionMatrix();
			m_needsToRecalculateProjectionMatrix = false;
		}

		/////////////////////////////////////////////////////////////////////

		void Camera::BuildRay(u32 x, u32 y, math::Vector3f& origin, math::Vector3f& direction)
		{
			math::Vector4u viewPort = Device::GetInstance()->GetDriver()->GetViewPort();
			
			sh::math::Matrix4f inverseProjMatrix = m_projectionMatrix.GetInversed();
			sh::math::Matrix4f inverseViewMatrix = m_viewMatrix.GetInversed();			
			origin = m_position;
	
			// 3d Normalised Device Coordinates	
			float xNorm = (2.0f * x) / viewPort.z - 1.0f;
			float yNorm = 1.0f - (2.0f * y) / viewPort.w;
			float zNorm = 1.0f;
			sh::math::Vector3f rayNds(xNorm, yNorm, zNorm);

			// 4d Homogeneous Clip Coordinates
			sh::math::Vector4f rayClip(rayNds.x, rayNds.y, -1.0f, 1.0f);

			// 4d Eye (Camera) Coordinates
			sh::math::Vector4f rayEye = inverseProjMatrix * rayClip;
			rayEye = sh::math::Vector4f(rayEye.x, rayEye.y, -1.0f, 0.0f);

			// 4d World Coordinates
			sh::math::Vector4f temp = inverseViewMatrix * rayEye;
			sh::math::Vector3f rayWorld(temp.x, temp.y, temp.z);
			// don't forget to normalise the vector at some point
			rayWorld.Normalize();
			direction = rayWorld;
		}

		/////////////////////////////////////////////////////////////////////

		const math::Matrix4f& Camera::GetViewMatrix()
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

		const math::Matrix4f& Camera::GetProjectionMatrix()
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

		const math::Matrix4f& Camera::GetViewProjectionMatrix()
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