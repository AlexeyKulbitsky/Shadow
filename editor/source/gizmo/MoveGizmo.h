#ifndef SHADOW_MOVE_GIZMO_INCLUDE
#define SHADOW_MOVE_GIZMO_INCLUDE

#include "Gizmo.h"

class MoveGizmo : public Gizmo
{
public:
	struct Axis
	{
		enum Type
		{
			X_AXIS = 0,
			Y_AXIS,
			Z_AXIS,

			XY_PLANE,
			XZ_PLANE,
			YZ_PLANE,

			COUNT,
			NONE
		};

		struct ModelInfo
		{
			sh::scene::ModelPtr model;
			sh::math::Vector4f currentColor;
			sh::math::Vector4f defaultColor;
			sh::math::Vector4f selectedColor;
		};

		std::vector<ModelInfo> models;
		sh::video::GpuParamsPtr params;
		sh::video::GpuParamVector4f color;
		sh::video::GpuParamMatrix4f wvpMatrix;
		bool active = false;
	};

	MoveGizmo();
	virtual ~MoveGizmo();
	virtual void Render() override;
	virtual void Process() override;
	virtual void OnMousePressed(sh::u32 x, sh::u32 y) override;
	virtual void OnMouseReleased(sh::u32 x, sh::u32 y) override;
	virtual void OnMouseMoved(sh::u32 x, sh::u32 y) override;
	virtual bool TryToSelect(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height) override;
	virtual bool IsActive() const override;


private:
	void CreateArrow(Axis::Type type);
	void CreatePlane(Axis::Type type);
	void Move(Axis::Type axis);
	void SetModifierActive(Axis::Type idx, bool active);

private:
	Axis m_axises[Axis::Type::COUNT];
	sh::video::MaterialPtr m_material;
	sh::video::CommandBufferPtr m_commandBuffer;
	Axis::Type m_activeModifier = Axis::NONE;
	bool m_mousePressed = false;
};

#endif