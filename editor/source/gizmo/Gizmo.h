#ifndef SHADOW_GIZMO_INCLUDE
#define SHADOW_GIZMO_INCLUDE

namespace sh
{
	namespace scene
	{
		class Model;
	}
}

class Gizmo
{
public:
	Gizmo();
	virtual ~Gizmo();
	void Render();

	sh::scene::Model* m_lineModel = nullptr;
};

#endif