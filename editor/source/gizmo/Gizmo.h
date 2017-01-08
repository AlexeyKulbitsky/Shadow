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
		sh::scene::Model* coneModel = nullptr;
		sh::video::Uniform* coneColorUniform = nullptr;
		bool active = false;
	};

	Gizmo();
	virtual ~Gizmo();
	void Render();

	void SetEnabled(bool enabled) { m_enabled = enabled; }
	bool IsEnabled() const { return m_enabled; }

	void SetEntity(sh::Entity* entity) { m_entity = entity; }

	void Move(Axis::Type axis);

	

	Axis m_axises[3];

	sh::scene::Model* m_lineModelX = nullptr;
	sh::scene::Model* m_coneModelX = nullptr;
	sh::scene::Model* m_lineModelY = nullptr;
	sh::scene::Model* m_coneModelY = nullptr;
	sh::scene::Model* m_lineModelZ = nullptr;
	sh::scene::Model* m_coneModelZ = nullptr;

	sh::Entity* m_entity = nullptr;
	bool m_enabled = false;
};

#endif