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

			COUNT
		};

		struct ModelInfo
		{
			sh::scene::ModelPtr model = nullptr;
			sh::video::Uniform* uniform = nullptr;
			sh::math::Vector4f color;
		};

		std::vector<ModelInfo> models;
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
	void CreateArrow(Axis::Type type);
	void CreatePlane(Axis::Type type);
	void Move(Axis::Type axis);

private:
	Axis m_axises[static_cast<sh::u32>(Axis::Type::COUNT)];
};

#endif