#ifndef SHADOW_CAMERA_INCLUDE
#define SHADOW_CAMERA_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace scene
	{
		class SHADOW_API Camera
		{
		public:
			Camera();
			~Camera();

			void Update();
			Camera* Clone();

			void SetPosition(const math::Vector3& pos);
			const math::Vector3& GetPosition() const { return m_position; }

			void SetRotation(const math::Quaternion& rot);
			const math::Quaternion& GetRotation() const { return m_rotation; }

			void SetProjection(f32 fov, f32 aspect, f32 nearPlane, f32 farPlane);
			void SetProjection(f32 fov, f32 width, f32 height, f32 nearPlane, f32 farPlane);
			void BuildRay(u32 x, u32 y, math::Vector3& origin, math::Vector3& direction);

			void SetFov(float fov);
			void SetFarPlane(float farPlane);
			void SetNearPlane(float nearPlane);
            void SetViewport(const math::Rect& viewport);

			float GetFov() const;
			float GetNearPlane() const;
			float GetFarPlane() const;
            const math::Rect& GetViewport() const;

			const math::Matrix4& GetViewMatrix();
			const math::Matrix4& GetProjectionMatrix();
			const math::Matrix4& GetViewProjectionMatrix();
			const math::Matrix4& Get2DProjectionMatrix() const { return m_2dProjectionMatrix; }
			const math::Matrix4& GetRotationMatrix() const { return m_rotationMatrix; }

			const math::Vector3& GetFrontVector() const { return m_frontVector; }
			const math::Vector3& GetRightVector() const { return m_rightVector; }
			const math::Vector3& GetUpVector() const { return m_upVector; }

		protected:
			void UpdateViewMatrix();
			void UpdateProjectionMatrix();
			void UpdateMatrixTransform();

		protected:
			math::Vector3 m_position;
			math::Quaternion m_rotation;

			float m_fovy;
			float m_nearDistance;
			float m_farDistance;
			float m_aspectRatio;
            
            math::Rect m_viewport;

			math::Matrix4 m_projectionMatrix;
			math::Matrix4 m_rotationMatrix;
			math::Matrix4 m_translationMatrix;
			math::Matrix4 m_viewMatrix;
			math::Matrix4 m_viewProjectionMatrix;
			math::Matrix4 m_2dProjectionMatrix;

			math::Vector3 m_frontVector;
			math::Vector3 m_upVector;
			math::Vector3 m_rightVector;

			bool m_needsToRecalculateViewMatrix;
			bool m_needsToRecalculateProjectionMatrix;
		};
	}
}

#endif
