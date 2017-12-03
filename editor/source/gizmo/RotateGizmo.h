#ifndef SHADOW_ROTATE_GIZMO_INCLUDE
#define SHADOW_ROTATE_GIZMO_INCLUDE

#include "Gizmo.h"

class RotateGizmo : public Gizmo
{
public:
	struct Axis
	{
		enum Type
		{
			X_AXIS = 0,
			Y_AXIS,
			Z_AXIS,

			COUNT,
			NONE
		};

		struct ModelInfo
		{
			sh::scene::ModelPtr model;
			sh::math::Vector4 currentColor;
			sh::math::Vector4 defaultColor;
			sh::math::Vector4 selectedColor;
		};

		std::vector<ModelInfo> models;
		sh::video::GpuParamsPtr params;
		sh::video::GpuParamVector4f color;
		sh::video::GpuParamMatrix4f wvpMatrix;
		bool active = false;
	};

	RotateGizmo();
	virtual ~RotateGizmo();
	virtual void Render() override;
	virtual void Process() override;
	virtual bool OnMouseMoved(sh::u32 x, sh::u32 y) override;
	virtual bool TryToSelect(sh::u32 x, sh::u32 y) override;
	virtual bool IsActive() const override;

	sh::Event<void, const sh::math::Quaternion&> rotationChanged;

private:
	void CreateCircle(Axis::Type type);
	void Rotate(Axis::Type axis);
	void SetModifierActive(Axis::Type idx, bool active);

private:
	Axis m_axises[Axis::Type::COUNT];
	sh::video::MaterialPtr m_material;
	sh::video::CommandBufferPtr m_commandBuffer;
	Axis::Type m_activeModifier = Axis::NONE;
};

#endif