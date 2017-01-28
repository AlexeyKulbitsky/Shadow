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

			COUNT
		};

		sh::scene::ModelPtr circleModel = nullptr;
		sh::video::Uniform* circleColorUniform = nullptr;
		sh::math::Vector3f localUp;
		bool active = false;
	};

	RotateGizmo();
	virtual ~RotateGizmo();
	virtual void Render() override;
	virtual void Process() override;
	virtual void OnMousePressed(sh::u32 x, sh::u32 y) override;
	virtual void OnMouseReleased(sh::u32 x, sh::u32 y) override;
	virtual void OnMouseMoved(sh::u32 x, sh::u32 y) override;
	virtual void TryToSelect(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height) override;
	virtual bool IsActive() const override;

private:
	void Rotate(Axis::Type axis);

private:
	Axis m_axises[4];
};

#endif