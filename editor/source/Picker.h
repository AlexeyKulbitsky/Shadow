#ifndef SHADOW_PICKER_INCLUDE
#define SHADOW_PICKER_INCLUDE

#include <Shadow.h>

class Gizmo;

class Picker
{
public:
	Picker();
	~Picker();
	void RegisterEntity(sh::Entity* entity);
	sh::Entity* TryToPick(sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height);
	void TryToPick(Gizmo* gizmo, sh::u32 x, sh::u32 y, sh::u32 width, sh::u32 height);

private:
	std::vector<sh::Entity*> m_entities;
	sh::video::RenderTechniquePtr m_renderTechnique = nullptr;
};

#endif