#include "Camera.h"
#include "SceneManager.h"

namespace sh
{
	namespace scene
	{
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