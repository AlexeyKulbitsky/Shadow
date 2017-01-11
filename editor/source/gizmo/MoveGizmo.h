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

			COUNT
		};

		sh::scene::Model* lineModel = nullptr;
		sh::video::Uniform* lineColorUniform = nullptr;
		sh::scene::Model* coneModel = nullptr;
		sh::video::Uniform* coneColorUniform = nullptr;
		bool active = false;
	};

	MoveGizmo();
	virtual ~MoveGizmo();
	virtual void Render() override;
	virtual void Process() override;
	virtual void OnMousePressed(sh::u32 x, sh::u32 y) override;
	virtual void OnMouseReleased(sh::u32 x, sh::u32 y) override;
	virtual void OnMouseMoved(sh::u32 x, sh::u32 y) override;
	virtual void TryToSelect(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height) override;
	virtual bool IsActive() const override;


private:
	void Move(Axis::Type axis);

private:
	Axis m_axises[3];
};

#endif