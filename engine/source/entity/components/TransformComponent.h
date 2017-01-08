#ifndef SHADOW_TRANSFORM_COMPONENT_INCLUDE
#define SHADOW_TRANSFORM_COMPONENT_INCLUDE

#include "../Component.h"

namespace sh
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent();
		virtual ~TransformComponent();
		virtual Component::Type GetType() const override { return Component::Type::TRANSFORM; }
		virtual void Load(const pugi::xml_node &node) override;
	
		const math::Matrix4f& GetWorldMatrix() const { return m_worldMatrix; }

		virtual void SetPosition(const math::Vector3f& position) { m_position = position; m_needsToRecalculateWorldMatrix = true; }
		virtual void SetRotation(const math::Quaternionf& rotation) { m_rotation = rotation; m_needsToRecalculateWorldMatrix = true; }
		virtual void SetScale(const math::Vector3f& scale) { m_scale = scale; m_needsToRecalculateWorldMatrix = true; }

		const math::Vector3f& GetPosition() const { return m_position; }
		const math::Quaternionf& GetRotation() const { return m_rotation; }
		const math::Vector3f& GetScale() const { return m_scale; }

		bool NeedsToRecalculateWorldMatrix() const { return m_needsToRecalculateWorldMatrix; }
		void SetNeedsToRecalculateWorldMatrix(bool value) { m_needsToRecalculateWorldMatrix = value; }

	protected:
		math::Matrix4f m_worldMatrix;

		math::Vector3f m_position;
		math::Quaternionf m_rotation;
		math::Vector3f m_scale;

		math::Matrix4f m_translateMatrix;
		math::Matrix4f m_rotationMatrix;
		math::Matrix4f m_scaleMatrix;

		bool m_needsToRecalculateWorldMatrix;

		friend class TransformSystem;
	};
}

#endif