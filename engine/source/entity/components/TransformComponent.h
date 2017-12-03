#ifndef SHADOW_TRANSFORM_COMPONENT_INCLUDE
#define SHADOW_TRANSFORM_COMPONENT_INCLUDE

#include "../Component.h"

namespace sh
{
	class SHADOW_API TransformComponent : public Component
	{
		SHADOW_OBJECT(TransformComponent)
		COMPONENT
	public:
		TransformComponent();
		virtual ~TransformComponent();

		static void RegisterObject();
	
		const math::Matrix4& GetWorldMatrix() const { return m_worldMatrix; }

		virtual void SetPosition(const math::Vector3& position) { m_position = position; m_needsToRecalculateWorldMatrix = true; }
		virtual void SetRotation(const math::Quaternion& rotation) { m_rotation = rotation; m_needsToRecalculateWorldMatrix = true; }
		virtual void SetScale(const math::Vector3& scale) { m_scale = scale; m_needsToRecalculateWorldMatrix = true; }

		const math::Vector3& GetPosition() const { return m_position; }
		const math::Quaternion& GetRotation() const { return m_rotation; }
		const math::Vector3& GetScale() const { return m_scale; }

		bool NeedsToRecalculateWorldMatrix() const { return m_needsToRecalculateWorldMatrix; }
		void SetNeedsToRecalculateWorldMatrix(bool value) { m_needsToRecalculateWorldMatrix = value; }

	protected:
		math::Matrix4 m_worldMatrix;

		math::Vector3 m_position;
		math::Quaternion m_rotation;
		math::Vector3 m_scale;

		math::Matrix4 m_translateMatrix;
		math::Matrix4 m_rotationMatrix;
		math::Matrix4 m_scaleMatrix;

		bool m_needsToRecalculateWorldMatrix;

		friend class TransformSystem;
	};
}

#endif