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

		Camera::~Camera()
		{

		}

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
			if (inputManager->IsMouseButtonPressed(MouseCode::BUTTON_LEFT))
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
			inputManager->SetMousePositionOld(current);

			if (m_needsToRecalculateViewMatrix)
			{
				UpdateViewMatrix();
				video::Driver* driver = Device::GetInstance()->GetDriver();
				driver->GetGlobalUniform(video::GlobalUniformName::CAMERA_VIEW_MATRIX)->Set(m_viewMatrix);
				driver->GetGlobalUniform(video::GlobalUniformName::CAMERA_VIEW_ROTATION_MATRIX)->Set(m_rotationMatrix);
				driver->GetGlobalUniform(video::GlobalUniformName::CAMERA_VIEW_PROJECTION_MATRIX)->Set((m_projectionMatrix * m_viewMatrix).GetTransposed());
				driver->GetGlobalUniform(video::GlobalUniformName::CAMERA_VIEW_ROTATION_PROJECTION_MATRIX)->Set((m_projectionMatrix * m_rotationMatrix).GetTransposed());
				m_needsToRecalculateViewMatrix = false;
			}
		}

		void Camera::SetPosition(const math::Vector3f& pos)
		{
			m_position = pos;
			m_needsToRecalculateViewMatrix = true;
		}

		void Camera::SetRotation(const math::Quaternionf& rot)
		{
			m_rotation = rot;
			m_needsToRecalculateViewMatrix = true;

			m_upVector = m_rotation * SceneManager::GetUpVector();
			m_frontVector = m_rotation * SceneManager::GetFrontVector();
			m_rightVector = m_rotation * SceneManager::GetRightVector();
		}

		void Camera::SetProjection(f32 fov, f32 aspect, f32 nearP, f32 farP)
		{
			m_fovy = fov;
			m_aspectRatio = aspect;
			m_nearDistance = nearP;
			m_farDistance = farP;

			UpdateProjectionMatrix();
			Device::GetInstance()->GetDriver()->GetGlobalUniform(video::GlobalUniformName::CAMERA_PROJECTION_MATRIX)->Set(m_projectionMatrix);
			m_needsToRecalculateProjectionMatrix = false;
		}

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

		void Camera::UpdateViewMatrix()
		{
			// Update rotation part
			m_rotationMatrix.SetIdentity();
			m_rotationMatrix.m[2][0] = -m_frontVector.x;
			m_rotationMatrix.m[2][1] = -m_frontVector.y;
			m_rotationMatrix.m[2][2] = -m_frontVector.z;

			m_rotationMatrix.m[0][0] = m_rightVector.x;
			m_rotationMatrix.m[0][1] = m_rightVector.y;
			m_rotationMatrix.m[0][2] = m_rightVector.z;

			m_rotationMatrix.m[1][0] = m_upVector.x;
			m_rotationMatrix.m[1][1] = m_upVector.y;
			m_rotationMatrix.m[1][2] = m_upVector.z;

			// Update translation part
			m_translationMatrix.SetIdentity();
			m_translationMatrix.m[0][3] = -m_position.x;
			m_translationMatrix.m[1][3] = -m_position.y;
			m_translationMatrix.m[2][3] = -m_position.z;

			m_viewMatrix = m_rotationMatrix * m_translationMatrix;
		}

		void Camera::UpdateProjectionMatrix()
		{
			m_projectionMatrix.SetPerspective(m_fovy, m_aspectRatio, m_nearDistance, m_farDistance);
		}

		void Camera::UpdateMatrixTransform()
		{
			if (m_needsToRecalculateViewMatrix)
				UpdateViewMatrix();

			if (m_needsToRecalculateProjectionMatrix)
				UpdateProjectionMatrix();
		}

		/*
		inline math::mat4f gl2shadow(const math::mat4f& im)
		{
			math::mat4f mat = im;
			math::vec4f axisX = im.getRow(0);
			math::vec4f axisY = im.getRow(1);
			math::vec4f axisZ = im.getRow(2);
			mat.setRow(0, -axisX);
			mat.setRow(1, axisZ);
			mat.setRow(2, axisY);

			return mat;
		}

		//////////////////////////////////////////////////////////////////////////

		inline math::mat4f shadow2gl(const math::mat4f& im)
		{
			math::mat4f mat = im;
			math::vec4f axisX = im.getRow(0);
			math::vec4f axisY = im.getRow(1);
			math::vec4f axisZ = im.getRow(2);
			mat.setRow(0, -axisX);
			mat.setRow(1, axisZ);
			mat.setRow(2, axisY);
			return mat;
		}
		
		Camera::Camera(const math::Vector3f& pos, const math::quatf& rot)
		{
			m_fovy = 0.f;
			m_nearDistance = 1.f;
			m_farDistance = 10000.f;
			m_worjdTransformChanged = true;
			m_projectionTransformChanged = true;
			m_aspectRatio = 0.f;
			//m_projectionOffCenter.set(math::vec2f(-1.f), math::vec2f(1.f));

			SetTransform(pos, rot);
			//SetFov(math::radians(45.f));
			SetNearDistance(0.5f);
			SetFarDistance(10000.f);
		}

		Camera::~Camera()
		{

		}

		void Camera::RefreshTransforms()
		{
			if (m_worjdTransformChanged)
			{
				// the View matrix is created by taking the absolute rotation and translation and then computing the inverse of that matrix.
				// we also have to take into account the axis change from jet to opengl coordinate system just before making the inverse.
				const math::quatf& rot = GetRotation();
				math::mat4f tmpViewTransform = rot.getAsMat4();
				math::vec3f pos = GetPosition();
				tmpViewTransform.setTranslation(pos);

				tmpViewTransform = shadow2gl(tmpViewTransform);

				// and now make the inverse in order to get a view matrix
				m_viewMatrix = math::inverse(tmpViewTransform);

				//refresh vectors
				m_frontVector = math::normalized(math::rotate(m_rotation, SceneManager::GetFrontVector()));
				m_upVector = math::normalized(math::rotate(m_rotation, SceneManager::GetUpVector()));
				m_rightVector = math::normalized(math::rotate(m_rotation, SceneManager::GetRightVector()));
			}

			if (m_projectionTransformChanged)
			{
				math::vec2u vpSize = GetViewportSize();
				float aspect = (float)(vpSize.x) / (float)(vpSize.y);				
				m_aspectRatio = aspect;
				
				m_projectionMatrix = ComputePerspectiveMatrix(m_fovy, aspect, m_nearDistance, m_farDistance);

				m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
			}

			m_worjdTransformChanged = false;
			m_projectionTransformChanged = false;
		}

		math::vec2u Camera::GetViewportSize() const
		{
			math::rectu vp = GetViewport();
			math::vec2u tmp = vp.getExtent() + math::vec2u(1, 1);
			return tmp;
		}

		math::mat4f Camera::ComputePerspectiveMatrix(float fovy, float aspect, float nearDist, float farDist)
		{
			float yHalfSize = math::tan(fovy * 0.5f) * nearDist;
			float xHalfSize = yHalfSize * aspect;

			float left = xHalfSize * m_projectionOffCenter.upperLeftCorner.x;
			float right = xHalfSize * m_projectionOffCenter.lowerRightCorner.x;
			float bottom = yHalfSize * m_projectionOffCenter.upperLeftCorner.y;
			float top = yHalfSize * m_projectionOffCenter.lowerRightCorner.y;

			return math::perspective(left, right, bottom, top, nearDist, farDist);
		}
		*/
	}
}