#ifndef SHADOW_GIZMO_INCLUDE
#define SHADOW_GIZMO_INCLUDE

#include <video\Uniform.h>

namespace sh
{
	class Entity;

	namespace scene
	{
		class Model;
	}
}

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

		sh::scene::Model* lineModel = nullptr;
		sh::video::Uniform* lineColorUniform = nullptr;
		bool active = false;
	};

	Gizmo();
	virtual ~Gizmo();
	virtual void Render();
	virtual void Process(){}
	virtual void OnMousePressed(sh::u32 x, sh::u32 y) {}
	virtual void OnMouseReleased(sh::u32 x, sh::u32 y) {}
	virtual void OnMouseMoved(sh::u32 x, sh::u32 y) {}
	virtual void TryToSelect(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height){}
	virtual bool IsActive() const { return false; }

	void SetEnabled(bool enabled) { m_enabled = enabled; }
	bool IsEnabled() const { return m_enabled; }
	void SetEntity(sh::Entity* entity);
	sh::Entity* GetEntity() { return m_entity; }


	sh::Entity* m_entity = nullptr;
	bool m_enabled = false;

private:
	Axis m_axises[3];
};

#endif