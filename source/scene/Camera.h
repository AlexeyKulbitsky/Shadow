#ifndef SHADOW_CAMERA_INCLUDE
#define SHADOW_CAMERA_INCLUDE

#include "../math/smath.h"

namespace sh
{
	namespace scene
	{
		/*
		class Camera
		{
		public:
			Camera(const math::vec3f& pos = math::vec3f(0), const math::quatf& rot = math::quatf());
			~Camera();

			void RefreshTransforms();

			void SetPosition(const math::vec3f& position);
			void SetRotation(const math::quatf& rotation);
			void SetTransform(const math::vec3f& position, const math::quatf& rotation);

			const math::vec3f& GetPosition() const { return m_position; }
			const math::quatf& GetRotation() const { return m_rotation; }

			const math::vec3f& GetFrontVector() const { return m_frontVector; }
			const math::vec3f& GetUpVector() const { return m_upVector; }
			const math::vec3f& GetRightVector() const { return m_rightVector; }

			const math::mat4f&	GetViewProjectionTransform() const { return m_viewProjectionMatrix; }
			const math::mat4f&	GetProjectionTransform() const { return m_projectionMatrix; }
			const math::mat4f&	GetViewTransform() const { return m_viewMatrix; }

			float GetFov() const { return m_fovy; }
			void SetFov(float fovy);

			float GetNearDistance() const { return m_nearDistance; }
			void SetNearDistance(float d);

			float GetFarDistance() const { return m_farDistance; }
			void SetFarDistance(float d);

			void				SetViewport(const math::rectu& vp);
			math::rectu			GetViewport() const { return m_viewport; }

			math::vec2u			GetViewportSize() const;

			float				GetAspectRatio() const { return m_aspectRatio; }

			// the default projection rect is [-1,-1] to [1,1]
			// be careful to keep aspect ratio in the scale when changing this
			void				SetProjectionOffCenter(const math::rectf& offset);
			const math::rectf&	GetProjectionOffCenter() const { return m_projectionOffCenter; }

		protected:
			math::mat4f			ComputePerspectiveMatrix(float fovy, float aspect, float nearDist, float farDist);

		private:
			math::vec3f m_position;
			math::quatf m_rotation;
			bool m_worjdTransformChanged;

			math::mat4f m_viewMatrix;
			math::mat4f m_projectionMatrix;
			math::mat4f m_viewProjectionMatrix;

			float m_fovy;
			float m_nearDistance;
			float m_farDistance;
			bool m_projectionTransformChanged;

			math::vec3f m_frontVector;
			math::vec3f	m_upVector;
			math::vec3f	m_rightVector;

			math::rectu			m_viewport;
			float				m_aspectRatio;
			math::rectf			m_projectionOffCenter;
		};

		inline void Camera::SetPosition(const math::vec3f& position)
		{
			m_position = position;
			m_worjdTransformChanged = true;
		}

		inline void Camera::SetRotation(const math::quatf& rotation)
		{
			m_rotation = rotation;
			m_worjdTransformChanged = true;
		}

		inline void Camera::SetTransform(const math::vec3f& position, const math::quatf& rotation)
		{
			m_position = position;
			m_rotation = rotation;
			m_worjdTransformChanged = true;
		}

		inline void Camera::SetFov(float fovy)
		{
			m_fovy = fovy;
			m_projectionTransformChanged = true;
		}

		inline void Camera::SetNearDistance(float d)
		{
			m_nearDistance = d;
			m_projectionTransformChanged = true;
		}

		inline void Camera::SetFarDistance(float d)
		{
			m_nearDistance = d;
			m_projectionTransformChanged = true;
		}

		inline void Camera::SetViewport(const math::rectu& vp)
		{
			m_viewport = vp;
			m_projectionTransformChanged = true;
		}

		inline void Camera::SetProjectionOffCenter(const math::rectf& offset)
		{
			m_projectionOffCenter = offset;
			m_projectionTransformChanged = true;
		}
		*/
	}
}

#endif
