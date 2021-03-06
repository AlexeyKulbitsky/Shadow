#ifndef SHADOW_SCALE_GIZMO_INCLUDE
#define SHADOW_SCALE_GIZMO_INCLUDE

#include "Gizmo.h"

class ScaleGizmo : public Gizmo
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

	ScaleGizmo();
	virtual ~ScaleGizmo();
	virtual void Render() override;
	virtual void Process() override;
	virtual bool OnMouseMoved(sh::u32 x, sh::u32 y) override;
	virtual bool TryToSelect(sh::u32 x, sh::u32 y) override;
	virtual bool IsActive() const override;

	sh::Event<void, const sh::math::Vector3&> scaleChanged;

private:
	void CreateArrow(Axis::Type type);
	void Scale(Axis::Type axis);
	void SetModifierActive(Axis::Type idx, bool active);

private:
	Axis m_axises[Axis::Type::COUNT];
	sh::video::MaterialPtr m_material;
	sh::video::CommandBufferPtr m_commandBuffer;
	Axis::Type m_activeModifier = Axis::NONE;
};

#endif