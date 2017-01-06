#ifndef SHADOW_GIZMO_INCLUDE
#define SHADOW_GIZMO_INCLUDE

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
	Gizmo();
	virtual ~Gizmo();
	void Render();

	void SetEntity(sh::Entity* entity) { m_entity = entity; }

	sh::scene::Model* m_lineModelX = nullptr;
	sh::scene::Model* m_coneModelX = nullptr;
	sh::scene::Model* m_lineModelY = nullptr;
	sh::scene::Model* m_coneModelY = nullptr;
	sh::scene::Model* m_lineModelZ = nullptr;
	sh::scene::Model* m_coneModelZ = nullptr;

	sh::Entity* m_entity = nullptr;
};

#endif