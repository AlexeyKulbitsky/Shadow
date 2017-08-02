#ifndef SHADOW_GIZMO_INCLUDE
#define SHADOW_GIZMO_INCLUDE

#include <Shadow.h>

class TransformWidget;

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
	virtual void OnMouseEvent(int x, int y, sh::MouseEventType type, sh::MouseCode code);
	virtual void OnMousePressed(sh::u32 x, sh::u32 y);
	virtual void OnMouseReleased(sh::u32 x, sh::u32 y);
	virtual void OnMouseMoved(sh::u32 x, sh::u32 y) {}
	virtual bool TryToSelect(sh::u32 x, sh::u32 y) { return false; }
	virtual bool IsActive() const { return false; }

	bool IsEnabled() const { return m_enabled; }
	void SetEntity(sh::Entity* entity);
	sh::Entity* GetEntity() { return m_entity; }
	void SetTransformWidget(const sh::SPtr<TransformWidget>& widget) { m_transformWidget = widget; }

protected:
	sh::Entity* m_entity = nullptr;
	bool m_enabled = false;
	bool m_mousePressed = false;
	sh::SPtr<TransformWidget> m_transformWidget;

private:
	Axis m_axises[3];
	sh::video::MaterialPtr m_material;
};

#endif