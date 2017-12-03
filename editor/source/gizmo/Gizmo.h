#ifndef SHADOW_GIZMO_INCLUDE
#define SHADOW_GIZMO_INCLUDE

#include <Shadow.h>

class TransformComponentWidget;

class Gizmo
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

		sh::scene::ModelPtr lineModel;
		sh::video::GpuParamsPtr params;
		sh::video::GpuParamMatrix4f wvpMtrix;
		sh::video::GpuParamVector4f color;
		bool active = false;
	};

	Gizmo();
	virtual ~Gizmo();
	virtual void Render();
	virtual void Process(){}
	virtual bool OnMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code);
	virtual bool OnMousePressed(sh::u32 x, sh::u32 y);
	virtual bool OnMouseReleased(sh::u32 x, sh::u32 y);
	virtual bool OnMouseMoved(sh::u32 x, sh::u32 y) { return false; }
	virtual bool TryToSelect(sh::u32 x, sh::u32 y) { return false; }
	virtual bool IsActive() const { return false; }

	bool IsEnabled() const { return m_enabled; }
	void SetEnabled(bool enabled) { m_enabled = enabled; }

	// Methods are shared between all children
	static void SetPosition(const sh::math::Vector3& position) { s_position = position; }
	static const sh::math::Vector3& GetPosition() { return s_position; }
	static void SetRotation(const sh::math::Quaternion& rotation) { s_rotation = rotation; }
	static const sh::math::Quaternion& GetRotation() { return s_rotation; }
	static void SetScale(const sh::math::Vector3& scale) { s_scale = scale; }
	static const sh::math::Vector3& GetScale() { return s_scale; }

protected:
	bool m_enabled = false;
	bool m_mousePressed = false;

	static sh::math::Vector3 s_position;
	static sh::math::Quaternion s_rotation;
	static sh::math::Vector3 s_scale;

private:
	Axis m_axises[3];
	sh::video::MaterialPtr m_material;
};

#endif